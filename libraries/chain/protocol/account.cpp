/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
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
#include <graphene/chain/protocol/account.hpp>

#include <fstream>
#include <iostream>
using namespace std;

namespace graphene { namespace chain {

/**
 * Names must comply with the following grammar (RFC 1035):
 * <domain> ::= <subdomain> | " "
 * <subdomain> ::= <label> | <subdomain> "." <label>
 * <label> ::= <letter> [ [ <ldh-str> ] <let-dig> ]
 * <ldh-str> ::= <let-dig-hyp> | <let-dig-hyp> <ldh-str>
 * <let-dig-hyp> ::= <let-dig> | "-"
 * <let-dig> ::= <letter> | <digit>
 *
 * Which is equivalent to the following:
 *
 * <domain> ::= <subdomain> | " "
 * <subdomain> ::= <label> ("." <label>)*
 * <label> ::= <letter> [ [ <let-dig-hyp>+ ] <let-dig> ]
 * <let-dig-hyp> ::= <let-dig> | "-"
 * <let-dig> ::= <letter> | <digit>
 *
 * I.e. a valid name consists of a dot-separated sequence
 * of one or more labels consisting of the following rules:
 *
 * - Each label is three characters or more
 * - Each label begins with a letter
 * - Each label ends with a letter or digit
 * - Each label contains only letters, digits or hyphens
 *
 * In addition we require the following:
 *
 * - All letters are lowercase
 * - Length is between (inclusive) GRAPHENE_MIN_ACCOUNT_NAME_LENGTH and GRAPHENE_MAX_ACCOUNT_NAME_LENGTH
 */

int write_string_into_file(const std::string & file_name, std::string src)
{
      std::ofstream os_write(file_name, std::ofstream::app);
      os_write<<src;
      os_write<<endl;
      os_write.close();
      return 0;
}

bool is_valid_name( const string& name )
{ try {
    const size_t len = name.size();

    if( len < GRAPHENE_MIN_ACCOUNT_NAME_LENGTH )
    {
          ilog( ".");
        return false;
    }

    if( len > GRAPHENE_MAX_ACCOUNT_NAME_LENGTH )
    {
          ilog( ".");
        return false;
    }

    size_t begin = 0;
    while( true )
    {
       size_t end = name.find_first_of( '.', begin );
       if( end == std::string::npos )
          end = len;
       if( (end - begin) < GRAPHENE_MIN_ACCOUNT_NAME_LENGTH )
       {
          idump( (name) (end)(len)(begin)(GRAPHENE_MAX_ACCOUNT_NAME_LENGTH) );
          return false;
       }
       switch( name[begin] )
       {
          case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
          case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
          case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':
          case 'y': case 'z':
             break;
          default:
          ilog( ".");
             return false;
       }
       switch( name[end-1] )
       {
          case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
          case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
          case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':
          case 'y': case 'z':
          case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
          case '8': case '9':
             break;
          default:
          ilog( ".");
             return false;
       }
       for( size_t i=begin+1; i<end-1; i++ )
       {
          switch( name[i] )
          {
             case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
             case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
             case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':
             case 'y': case 'z':
             case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
             case '8': case '9':
             case '-':
                break;
             default:
          ilog( ".");
                return false;
          }
       }
       if( end == len )
          break;
       begin = end+1;
    }
    return true;
} FC_CAPTURE_AND_RETHROW( (name) ) }

bool is_cheap_name( const string& n )
{
   bool v = false;
   for( auto c : n )
   {
      if( c >= '0' && c <= '9' ) return true;
      if( c == '.' || c == '-' || c == '/' ) return true;
      switch( c )
      {
         case 'a':
         case 'e':
         case 'i':
         case 'o':
         case 'u':
         case 'y':
            v = true;
      }
   }
   if( !v )
      return true;
   return false;
}

void account_options::validate() const
{
   auto needed_witnesses = num_witness;
   auto needed_committee = num_committee;

   for( vote_id_type id : votes )
      if( id.type() == vote_id_type::witness && needed_witnesses )
         --needed_witnesses;
      else if ( id.type() == vote_id_type::committee && needed_committee )
         --needed_committee;

   FC_ASSERT( needed_witnesses == 0 && needed_committee == 0,
              "May not specify fewer witnesses or committee members than the number voted for.");
}

share_type account_create_operation::calculate_fee( const fee_parameters_type& k )const
{
   auto core_fee_required = k.basic_fee;

   if( !is_cheap_name(name) )
      core_fee_required = k.premium_fee;

   // Authorities and vote lists can be arbitrarily large, so charge a data fee for big ones
   auto data_fee =  calculate_data_fee( fc::raw::pack_size(*this), k.price_per_kbyte ); 
   core_fee_required += data_fee;

   return core_fee_required;
}


void account_create_operation::validate()const
{
   FC_ASSERT( fee.amount >= 0 );
   FC_ASSERT( is_valid_name( name ) );
   FC_ASSERT( referrer_percent <= GRAPHENE_100_PERCENT );
   FC_ASSERT( owner.num_auths() != 0 );
   FC_ASSERT( owner.address_auths.size() == 0 );
   FC_ASSERT( active.num_auths() != 0 );
   FC_ASSERT( active.address_auths.size() == 0 );
   FC_ASSERT( !owner.is_impossible(), "cannot create an account with an imposible owner authority threshold" );
   FC_ASSERT( !active.is_impossible(), "cannot create an account with an imposible active authority threshold" );
   options.validate();
   if( extensions.value.owner_special_authority.valid() )
      validate_special_authority( *extensions.value.owner_special_authority );
   if( extensions.value.active_special_authority.valid() )
      validate_special_authority( *extensions.value.active_special_authority );
   if( extensions.value.buyback_options.valid() )
   {
      FC_ASSERT( !(extensions.value.owner_special_authority.valid()) );
      FC_ASSERT( !(extensions.value.active_special_authority.valid()) );
      FC_ASSERT( owner == authority::null_authority() );
      FC_ASSERT( active == authority::null_authority() );
      size_t n_markets = extensions.value.buyback_options->markets.size();
      FC_ASSERT( n_markets > 0 );
      for( const asset_id_type m : extensions.value.buyback_options->markets )
      {
         FC_ASSERT( m != extensions.value.buyback_options->asset_to_buy );
      }
   }
}




share_type account_update_operation::calculate_fee( const fee_parameters_type& k )const
{
   auto core_fee_required = k.fee;  
   if( new_options )
      core_fee_required += calculate_data_fee( fc::raw::pack_size(*this), k.price_per_kbyte );
   return core_fee_required;
}

void account_update_operation::validate()const
{
   FC_ASSERT( account != GRAPHENE_TEMP_ACCOUNT );
   FC_ASSERT( fee.amount >= 0 );
   FC_ASSERT( account != account_id_type() );

   bool has_action = (
         owner.valid()
      || active.valid()
      || new_options.valid()
      || extensions.value.owner_special_authority.valid()
      || extensions.value.active_special_authority.valid()
      );

   FC_ASSERT( has_action );

   if( owner )
   {
      FC_ASSERT( owner->num_auths() != 0 );
      FC_ASSERT( owner->address_auths.size() == 0 );
      FC_ASSERT( !owner->is_impossible(), "cannot update an account with an imposible owner authority threshold" );
   }
   if( active )
   {
      FC_ASSERT( active->num_auths() != 0 );
      FC_ASSERT( active->address_auths.size() == 0 );
      FC_ASSERT( !active->is_impossible(), "cannot update an account with an imposible active authority threshold" );
   }

   if( new_options )
      new_options->validate();
   if( extensions.value.owner_special_authority.valid() )
      validate_special_authority( *extensions.value.owner_special_authority );
   if( extensions.value.active_special_authority.valid() )
      validate_special_authority( *extensions.value.active_special_authority );
}

share_type account_upgrade_operation::calculate_fee(const fee_parameters_type& k) const
{
   if( upgrade_to_lifetime_member )
      return k.membership_lifetime_fee;
   return k.membership_annual_fee;
}


void account_upgrade_operation::validate() const
{
   FC_ASSERT( fee.amount >= 0 );
}

void account_transfer_operation::validate()const
{
   FC_ASSERT( fee.amount >= 0 );
}

void smart_contract_upload_operation::validate()const
{
   FC_ASSERT( 0 >= 0 );//TODO: there are more condition to be checked // by Victor Sun
   std::cout<<"the validate function of smart_contract_upload_operation is being called!"<<std::endl;
   //将合约类的源代码写入到文件中去，这个后去迭代改造需要修改保存到数据库中去
//   write_string_into_file("./test_sc.wren", this->smart_contract); 
}

void smart_contract_activate_operation::validate()const
{
   FC_ASSERT( 0 >= 0 );//TODO: there are more condition to be checked // by Victor Sun
   std::cout<<"the validate function of smart_contract_activate_operation is being called!"<<std::endl;
}

void smart_contract_call_operation::validate()const //该函数需要对结构体中的成员变量值做修改，故只能将const注释掉,函数声明处也作相应处理 modified by Victor Sun
{
   FC_ASSERT( 0 >= 0 );//TODO: there are more condition to be checked
   std::cout<<"the validate function of smart_contract_call_operation is being called!"<<std::endl;
//   int len = 0;
   //
//   char* class_name_init = (char *)(this->class_name).data();

 //  cout<<this->class_name<<endl;
//   len = (this->class_name).length();
//   char* class_name_init = (char *)malloc((len+1)*sizeof(char));
//   (this->class_name).copy(class_name_init, len, 0);



//   cout<<"len = (this->class_name).length() = "<<len<<endl;
//   cout<<"class_name_init =  "<<class_name_init<<endl;

//   class_name_init[len] = '\0';
//   cout<<"class_name_init[4] =  "<<class_name_init[4]<<endl;
   //
   //
   /*
//   len = (this->method_name).length();
   char* method_name_init = (char *)(this->method_name).data();

   //
   //将调用函数的参数列表转换格式
//   len = (this->input_parameters).length();
   char* input_parameters_init = (char *)(this->input_parameters).data();
//   (this->input_parameters).copy(input_parameters_init, len, 0);
//   input_parameters_init[len] = '\0';
   cout<<"input_parameters_init = "<<input_parameters_init<<endl;
//

   container_of_all_smart_contracts_running _the_container;//定义一个容器
   _the_container.insert_an_initialized_smart_contract_object( caller_init, smart_contract_code_init );//将合约放入容器
//   _the_container.visit_container( );//遍历容器（其实只有一个元素）


   int call_index = 1;
   _the_container.call_smart_contract_in_container(call_index, class_name_init, method_name_init, input_parameters_init);

//   free(class_name_init);
//   free(method_name_init);
//   free(input_parameters_init);
   //调用结束之后，从容器中删除这个smart contract
   _the_container.delete_the_initialized_smart_contract_object( call_index );

   //_the_container这个容器被删除是要调用构造函数么？


*/

//end calling
   std::cout<<"========================================================"<<std::endl;

//   output_parameter = input_parameter + 101010101;
}

void data_digest_upload_operation::validate()const
{
   FC_ASSERT( 0 >= 0 );//TODO: there are more condition to be checked
   std::cout<<"the validate function of data_digest_upload_operation is being called!"<<std::endl;
}


} } // graphene::chain
