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
