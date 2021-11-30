wxflags = `wx-config-2.8 --cxxflags`
wxlibs = `wx-config-2.8 --libs`

lua= -L. -llua50
src = src/
obj = obj/

#
agents2007: $(src)agents2007.cpp $(obj)engine.o $(obj)xml.o $(obj)tinyxml.o $(obj)tinystr.o $(obj)tinyxmlerror.o $(obj)tinyxmlparser.o
	g++ $(src)agents2007.cpp $(obj)engine.o $(obj)xml.o $(obj)tinyxml.o $(obj)tinystr.o $(obj)tinyxmlerror.o $(obj)tinyxmlparser.o $(wxflags) $(wxlibs) -o agents2007 -Iinclude $(lua)

$(obj)engine.o: $(src)engine.cpp 
	g++ -c $(src)engine.cpp $(wxflags) -o $(obj)engine.o -Iinclude
	
$(obj)xml.o: $(src)xml.cpp  
	g++ -c $(src)xml.cpp -o $(obj)xml.o -Iinclude

$(obj)tinyxml.o: $(src)tinyxml.cpp
	g++ -c $(src)tinyxml.cpp -o $(obj)tinyxml.o
    
$(obj)tinystr.o: $(src)tinystr.cpp
	g++ -c $(src)tinystr.cpp -o $(obj)tinystr.o
	
$(obj)tinyxmlerror.o: $(src)tinyxmlerror.cpp
	g++ -c $(src)tinyxmlerror.cpp -o $(obj)tinyxmlerror.o
	
$(obj)tinyxmlparser.o: $(src)tinyxmlparser.cpp	
	g++ -c $(src)tinyxmlparser.cpp -o $(obj)tinyxmlparser.o

clean:
	rm -f $(obj)*.o agents2007

pack:
	tar -cf xnagye00.tar ../xnagye00
	gzip xnagye00.tar
#
