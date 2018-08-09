/*
 * Copyright (c) 2018- μNEST Foundation, and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

// use berkeley db to store objects
//
#include <db_cxx.h> 
#include <graphene/db/index.hpp>  

namespace graphene { namespace db {


class bdb
{
public:
	bdb();

	// Constructor requires a path to the database,
	// and a database name.
	bdb(const char* path, bool allow_duplicate = false);

	// Our destructor just calls our private close method.
	~bdb() { close(); }

	inline Db &getDb() { return _db; }
	Db* operator ->() { return &_db; }

	void open( const char* path, bool allow_duplicate = false );
	void close();
	bool isOpen() const { return _isOpen; }
	
private:
	Db _db;
	std::string _dbFileName;
	u_int32_t _cFlags;
	bool _isOpen;
};

// a singleton to hold a berkeley db environment.

class bdb_env
{
public:
	~bdb_env()
	{
		if (_dbenv != nullptr)
		{
			delete _dbenv;
			_dbenv = nullptr;
		}
	}
	static bdb_env& getInstance()
	{
		static bdb_env m_instance;
		return m_instance;
	}

	DbEnv* getDbEnv() const
	{
		return _dbenv;
	}

	operator DbEnv* () const
	{
		return _dbenv;
	}

	void init(const char* home, const char* data_dir);

private:
	bdb_env();

private:
	DbEnv *_dbenv;
};

template<typename ObjectType>
class bdb_secondary_index;

template<typename ObjectType>
class bdb_index : public index
{
public:
	typedef ObjectType     object_type;

	bdb_index()
	{
		std::string file = fc::to_string(object_type::space_id) + "." + fc::to_string(object_type::type_id);
		_bdb.open(file.c_str());
	}


	// this function is just an override, do not rely on the returned obj
	// please use create_db() instead
	virtual const object&  create(const std::function<void(object&)>& constructor) override
	{
		object_type obj;
		obj.id = get_next_id();
		constructor(static_cast<object_type&>(obj));

		vector<char> v = obj.pack();
		Dbt data(v.data(), v.size());
		fc::string id = fc::to_string(obj.id.instance());
		Dbt key((void*)id.c_str(), id.size() + 1);

		auto ret = _bdb->put(nullptr, &key, &data, 0);

		FC_ASSERT(!ret, "Could not create object and insert into berkeley DB");
		use_next_id();
		return obj; // this return obj is not reliable,
	}

	virtual std::unique_ptr<object> create_db(const std::function<void(object&)>& constructor) override
	{
		std::unique_ptr<object> obj(new object_type());
		obj->id = get_next_id();
		constructor(static_cast<object_type&>(*obj));

		vector<char> v = obj->pack();
		Dbt data(v.data(), v.size());
		fc::string id = fc::to_string(obj->id.instance());
		Dbt key((void*)id.c_str(), id.size() + 1);

		auto ret = _bdb->put(nullptr, &key, &data, 0);
		// _bdb->sync(0); // write to disk

		FC_ASSERT(!ret, "Could not create object and insert into berkeley DB");
		use_next_id();
		return obj;
	}

	// this function is just an override, do not rely on the returned obj
	// please use insert_db() instead
	virtual const object& insert(object&& obj) override
	{
		object_type *pObj = dynamic_cast<ObjectType*>(&obj);
		assert(nullptr != pObj);

		vector<char> v = static_cast<ObjectType&>(obj).pack();
		fc::string id = fc::to_string(obj.id.instance());
		Dbt key((void*)id.c_str(), id.size()+1);
		Dbt data(v.data(), v.size());

		int ret = _bdb->put(nullptr, &key, &data, 0);

		FC_ASSERT(!ret, "Could not create object and insert into berkeley DB");
		return obj; // This is not allowed, but we have to return a lvalue.
	}

	virtual bool insert_db(object&& obj) override
	{
		object_type *pObj = dynamic_cast<ObjectType*>(&obj);
		assert(nullptr != pObj);

		vector<char> v = static_cast<ObjectType&>(obj).pack();
		fc::string id = fc::to_string(obj.id.instance());
		Dbt key((void*)id.c_str(), id.size() + 1);
		Dbt data(v.data(), v.size());

		int ret = _bdb->put(nullptr, &key, &data, 0);

		FC_ASSERT(!ret, "Could not insert object into berkeley db. ret=${ret}", ("ret", ret) );
		return !ret;
	}

	virtual void modify(const object& obj, const std::function<void(object&)>& modify_callback) override
	{
		assert(nullptr != dynamic_cast<const ObjectType*>(&obj));

		Dbt data;;
		fc::string id = fc::to_string(obj.id.instance());
		Dbt key((void*)id.c_str(), id.size() + 1);
		
		auto ret = _bdb->get( nullptr, &key, &data, 0);
		FC_ASSERT(!ret, "Could not modify object, not found");

		object_type objFromDB;
		fc::raw::unpack<object_type>((const char*)data.get_data(), data.get_size(), objFromDB);

		modify_callback(objFromDB);

		vector<char> v = objFromDB.pack();
		Dbt modData(v.data(), v.size());
		ret = _bdb->put(nullptr, &key, &modData, 0);

		FC_ASSERT(!ret, "Could not modify object from berkeley db");
	}

	virtual void remove_db(object_id_type id) override
	{
		fc::string k = fc::to_string(id.instance());
		Dbt key((void*)k.c_str(), k.size() + 1);
		auto status = _bdb->del(nullptr, &key, 0);
		FC_ASSERT(!status, "Could not delete object from berkeley db, id:${id}", ("id", (std::string)id));
	}

	virtual void remove(const object& obj) override
	{
		assert(nullptr != dynamic_cast<const ObjectType*>(&obj));
		fc::string k = fc::to_string(obj.id.instance());
		Dbt key((void*)k.c_str(), k.size() + 1);

		auto status = _bdb->del(nullptr, &key, 0);
		FC_ASSERT(!status, "Could not delete object from berkeley db");
	}

	// callers have to free the returned obj 
	// do not use this function for this class, use find_db instead.
	virtual const object* find(object_id_type id)const override
	{
		assert(id.space() == object_type::space_id);
		assert(id.type() == object_type::type_id);
		
		return nullptr;
	}

	virtual std::unique_ptr<object> find_db(object_id_type id) override
	{
		fc::string k = fc::to_string(id.instance());
		Dbt key((void*)k.c_str(), k.size() + 1);
		Dbt data;
		auto ret = _bdb->get(nullptr, &key, &data, 0);

		if (!ret) //  == DB_NOTFOUND 
			return std::unique_ptr<object_type>(nullptr);

		std::unique_ptr<object> obj(new object_type());  
		fc::raw::unpack<object_type>((const char*)data.get_data(), data.get_size(), dynamic_cast<object_type&>(*obj));

		return obj;
	}

	virtual void inspect_all_objects(std::function<void(const object&)> inspector)const override
	{
		;
	}

	virtual fc::uint128 hash()const override {
		fc::uint128 result;
		// for (const auto& ptr : _objects)
		//	result += ptr->hash();

		return result;
	}

	void flush()
	{
		_bdb->sync(0);
	}

	void save_next_id()
	{
		const char* k = "_next_id";
		Dbt key((void*)(k), strlen(k)+1);

		object_id_type next_id = get_next_id();
		Dbt data(&next_id, sizeof(next_id));
		_bdb->put(nullptr, &key, &data, 0);
		_bdb->sync(0);

		std::cout << "Berkeley DB: Saved next_id: " << (std::string)next_id << std::endl;
	}

	void load_next_id()
	{
		const char* k = "_next_id";
		Dbt key((void*)k, (u_int32_t)strlen(k) + 1);

		Dbt data;
		int ret = _bdb->get(nullptr, &key, &data, 0);
		if (!ret)
		{
			object_id_type next_id;
			memcpy(&next_id, data.get_data(), data.get_size());
			set_next_id(next_id);
			
			std::cout << "Berkeley DB: Loaded next_id: " << (std::string)next_id << std::endl;
		}
	}
	void save_data_version(fc::sha256& version)
	{
		const char* k = "_data_version";
		Dbt key((void*)(k), strlen(k) + 1);

		Dbt data(&version, sizeof(fc::sha256));
		_bdb->put(nullptr, &key, &data, 0);
		_bdb->sync(0);

		std::cout << "Berkeley DB: Saved data_version: " << (std::string)version << std::endl;
	}

	bool load_data_version(fc::sha256& version)
	{
		const char* k = "_data_version";
		Dbt key((void*)k, (u_int32_t)strlen(k) + 1);
		 
		Dbt data;
		data.set_data(&version);
		data.set_ulen(sizeof(fc::sha256));
		data.set_flags(DB_DBT_USERMEM);
		int ret = _bdb->get(nullptr, &key, &data, 0);
		if (!ret)
		{ 
			std::cout << "Berkeley DB: Loaded data_version: " << (std::string)version << std::endl; 
		}
		return !ret;
	}

	int add_bdb_secondary_index(bdb_secondary_index<ObjectType>* secondary_index, int(*callback)(Db*, const Dbt*, const Dbt*, Dbt*))
	{
		std::unique_ptr<bdb_secondary_index<ObjectType>> sindexptr(secondary_index);
		_s_indexs.push_back(std::move(sindexptr));

		int pos = _s_indexs.size() - 1; 

		int ret = _bdb->associate(nullptr, *secondary_index, callback, 0);
		if (!ret)
		{
			std::cout << "Berkeley DB: add_secondary_index(): " << std::endl;
			return pos;
		}
		else
		{
			std::cerr << "Berkeley DB: failed add_secondary_index(): " << std::endl;
		}
		return -1;
	}

	std::unique_ptr<object_type> find_db_secondary(int pos, void* k, size_t size) 
	{
		Dbt key(k, size);
		Dbt data;
		auto ret = _s_indexs[pos]->get(nullptr, &key, &data, 0);

		if (!ret) //  == DB_NOTFOUND 
			return std::unique_ptr<object_type>(nullptr);

		std::unique_ptr<object_type> obj(new object_type());
		fc::raw::unpack<object_type>((const char*)data.get_data(), data.get_size(), *obj);

		return obj;
	} 

private:
	bdb _bdb;
	vector<std::unique_ptr<bdb_secondary_index<ObjectType>>> _s_indexs;

	bool _raw_data;  // no pack/unpack, put the raw struct
};


template<typename ObjectType>
class primary_index<bdb_index<ObjectType>> : public bdb_index<ObjectType>, public base_primary_index
{
public:
	typedef bdb_index<ObjectType> DerivedIndex;
	typedef typename DerivedIndex::object_type object_type;

	primary_index(object_database& db)
		:base_primary_index(db), _next_id(object_type::space_id, object_type::type_id, 0) {}

	virtual uint8_t object_space_id()const override
	{
		return object_type::space_id;
	}

	virtual uint8_t object_type_id()const override
	{
		return object_type::type_id;
	}

	virtual object_id_type get_next_id()const override { return _next_id; }
	virtual void           use_next_id()override { ++_next_id.number; }
	virtual void           set_next_id(object_id_type id)override { _next_id = id; }

	fc::sha256 get_object_version()const
	{
		std::string desc = "1.0";//get_type_description<object_type>();
		return fc::sha256::hash(desc);
	}

	virtual void open(const path& db)override
	{
		fc::sha256 open_ver;

		DerivedIndex::load_next_id();
		if( DerivedIndex::load_data_version(open_ver)) 
			FC_ASSERT(open_ver == get_object_version(), "Incompatible Version, the serialization of objects in this db has changed");
	}

	virtual void save(const path& db) override
	{
		DerivedIndex::flush();
		DerivedIndex::save_next_id();
		auto ver = get_object_version();
		DerivedIndex::save_data_version(ver);
	}

	virtual const object&  load(const std::vector<char>& data)override
	{
		const auto& result = DerivedIndex::insert(fc::raw::unpack<object_type>(data));
		for (const auto& item : _sindex)
			item->object_inserted(result);
		return result;
	}


	virtual const object&  create(const std::function<void(object&)>& constructor)override
	{
		const auto& result = DerivedIndex::create(constructor);
		for (const auto& item : _sindex)
			item->object_inserted(result);
		on_add(result);
		return result;
	}

	virtual const object& insert(object&& obj) override
	{
		const auto& result = DerivedIndex::insert(std::move(obj));
		for (const auto& item : _sindex)
			item->object_inserted(result);
		on_add(result);
		return result;
	}

	virtual void  remove(const object& obj) override
	{
		for (const auto& item : _sindex)
			item->object_removed(obj);
		on_remove(obj);
		DerivedIndex::remove(obj);
	}

	virtual void  remove_db(object_id_type id) override
	{
		DerivedIndex::remove_db(id);
	}

	virtual void modify(const object& obj, const std::function<void(object&)>& m)override
	{
		save_undo(obj);
		for (const auto& item : _sindex)
			item->about_to_modify(obj);
		DerivedIndex::modify(obj, m);
		for (const auto& item : _sindex)
			item->object_modified(obj);
		on_modify(obj);
	}

	virtual void add_observer(const shared_ptr<index_observer>& o) override
	{
		_observers.emplace_back(o);
	}

	virtual void object_from_variant(const fc::variant& var, object& obj, uint32_t max_depth)const override
	{
		object_id_type id = obj.id;
		object_type* result = dynamic_cast<object_type*>(&obj);
		FC_ASSERT(result != nullptr);
		fc::from_variant(var, *result, max_depth);
		obj.id = id;
	}

	virtual void object_default(object& obj)const override
	{
		object_id_type id = obj.id;
		object_type* result = dynamic_cast<object_type*>(&obj);
		FC_ASSERT(result != nullptr);
		(*result) = object_type();
		obj.id = id;
	}

private:
	object_id_type _next_id;
};

template<typename ObjectType>
class bdb_secondary_index
{ 
public:
	typedef ObjectType     object_type;

	bdb_secondary_index(bdb_secondary_index&& rv) : _bdb( rv._bdb)
	{		
	}

	bdb_secondary_index(std::string tag, bool duplicate)
	{
		std::string file = fc::to_string(object_type::space_id) + "." + fc::to_string(object_type::type_id) + "." + tag;
		_bdb.open(file.c_str(), duplicate);
		FC_ASSERT( _bdb.isOpen(), "Berkeley DB: Could not open file '${file}'", ("file", file));
	}

	operator Db* ()
	{
		return &_bdb.getDb();
	}
	Db* operator->()
	{
		return &_bdb.getDb();
	}

private:
	bdb _bdb;
};

}
}
