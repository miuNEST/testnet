class Contract {
    member_variables{[]}
    
    get_member_variables() {
        return member_variables
    }

    save_member_variables_into_c() {
        System.print("save_member_variables_into_c()")
        var lists = get_member_variables()
        var string = maptostring(lists)
        System.print(string)
        setStateToString(string)
        return true
    }
    
    reassemble_member_variables_from_c(){
        System.print("reassemble_member_variables_from_c()")
        var string = getStateFromString()
        var x =stringtomap(string)
        System.print(x)
        return x
    }

    static reassemble_state(){
        System.print("static reassemble_member_variables_from_c()")
        var string = getStateFromString_ex()
        System.print(string)
        var x =stringtomap_ex(string)
        return x
    }
    
    maptostring(lists){
    	var a="["
		for (i in lists){
    	    var ret4 = wrenToString(i)
    	    if (ret4==3 ){
            a = a+"\""+i+"\","
    	}else if(ret4 ==4){
            System.print("unknown type")
    	}else if(ret4 ==1){
        	a = a+i.toString+","
    	}else{
            a = a+i.toString+","
        }
      }
      a =a+"]"
      a =a.replace(",]","]")
      return a
   }

    stringtomap(string){
    	var p = string.replace("[","")
		var q = p.replace("]","")
		var t =q.split(",")
        var z =[]
        for (i in t){
            var rc = mapToGettype(i)
            if(rc ==3){
                z.add(Num.fromString(i))
            }else if(rc ==4){
                z.add(i.replace("\"",""))
            }else{
                z.add(rc)
            } 
        } 
		return z
   }

   static stringtomap_ex(string){
    	var p = string.replace("[","")
		var q = p.replace("]","")
		var t =q.split(",")
        var z =[]
        for (i in t){
            var rc = mapToGettype_ex(i)
            if(rc ==3){
                z.add(Num.fromString(i))
            }else if(rc ==4){
                z.add(i.replace("\"",""))
            }else{
                z.add(rc)
            } 
        } 
		return z
   }
    
   foreign wrenToString(state)
   foreign mapToGettype(state)
   foreign setStateToString(a)
   foreign getStateFromString()
   foreign static getStateFromString_ex()
   foreign static mapToGettype_ex(state)
    
}
