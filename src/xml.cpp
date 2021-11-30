/*******************************************************************************
**                          A G E N T S   2 0 0 7                              *
********************************************************************************
**  projekt:      agents2007                                                   *
**  autori:       xsusma00, xsmejk13, xnagye00, xtrnko00                       *
*******************************************************************************/

#include <cstdlib>
#include <iostream>
//#include "stdafx.h" // <-- you MIGHT need this
#include "tinyxml.h" // <-- you definitely need this ;)
#include "engine.h"
//#include "xml.h"

using namespace std;

int ulozSkript(string &skript, string subor)
{
  TiXmlDocument doc;
  TiXmlElement * root = new TiXmlElement("skript");
  TiXmlText * text = new TiXmlText( skript.c_str() );
  root->LinkEndChild( text );
  doc.LinkEndChild( root );
  doc.SaveFile(subor.c_str());
return 0;
}


int nacitajSkript(string &ret,string subor)
{
	TiXmlDocument doc(subor.c_str());
	if (!doc.LoadFile()) { cout<< "Nenacitalo sa to"<< endl; return 1;}
	TiXmlHandle hDoc(&doc);  //handle na loadnuty dokument
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	pElem=hDoc.FirstChildElement().Element();
	if (!pElem)  { cout<< "Nenasiel sa element"<< endl; return 1;}
	ret=pElem->Value();
  //std::cout<< "Nacitali sme:" << ret <<endl;
  const char *pText=pElem->GetText();
  ret=pText;
  return 0;
  //cout<< "Nacitany text:" << std::endl << ret << endl;
}

// a utility function defining a very simple method to indent a line of text
const char * getIndent( unsigned int numIndents )
{
    static const char * pINDENT = "                                      + ";
    static const unsigned int LENGTH = strlen( pINDENT );

    if ( numIndents > LENGTH ) numIndents = LENGTH;

    return &pINDENT[ LENGTH-numIndents ];
}

void dump_to_stdout( TiXmlNode * pParent, unsigned int indent = 0 )
{
    if ( !pParent ) return;

    TiXmlText *pText;
    int t = pParent->Type();
    printf( "%s", getIndent( indent));

    switch ( t )
    {
    case TiXmlNode::DOCUMENT:
        printf( "Document" );
        break;

    case TiXmlNode::ELEMENT:
        printf( "Element \"%s\"", pParent->Value() );
        break;

    case TiXmlNode::COMMENT:
        printf( "Comment: \"%s\"", pParent->Value());
        break;

    case TiXmlNode::UNKNOWN:
        printf( "Unknown" );
        break;

    case TiXmlNode::TEXT:
        pText = pParent->ToText();
        printf( "Text: [%s]", pText->Value() );
        break;

    case TiXmlNode::DECLARATION:
        printf( "Declaration" );
        break;
    default:
        break;
    }
    printf( "\n" );

    TiXmlNode * pChild;

    for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        dump_to_stdout( pChild, indent+2 );
    }
}


void savescena( SCENA *scena, string name)
{
    // This example courtesy of polocolege

    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );

    TiXmlElement * element = new TiXmlElement( "SCENA" );
    doc.LinkEndChild( element );
    element->SetAttribute("x", scena->rozmerx);
    element->SetAttribute("y", scena->rozmery);

    TiXmlElement * windowsNode = new TiXmlElement( "SURADNICE" );
		element->LinkEndChild( windowsNode );

		//list<WindowSettings>::iterator iter;

    for (int j=0; j < scena->rozmery; j++)
    {
		for (int i=0; i < scena->rozmerx; i++)
		{
//			const WindowSettings& w=*iter;
            if(scena->pole[j*scena->rozmerx+i].nova!=0)
            {
			TiXmlElement * window;
			window = new TiXmlElement( "X:Y" );
			windowsNode->LinkEndChild( window );
			window->SetAttribute("x", i);
			window->SetAttribute("y", j);
			window->SetAttribute("content", scena->pole[j*scena->rozmerx+i].nova);
            }
		}
    }


    TiXmlElement * agentNode = new TiXmlElement( "AGENTS" );
		element->LinkEndChild( agentNode );


    for(IterAgents iter=scena->agents.begin(); iter != scena->agents.end(); iter++)
    {
    //(*iter).krok();
			TiXmlElement * agent;
			agent = new TiXmlElement( "AGENT" );
			agentNode->LinkEndChild( agent );
			agent->SetAttribute("x", (*iter).posx);
			agent->SetAttribute("y", (*iter).posy);
			agent->SetAttribute("smer", (*iter).smerpohybu);
			agent->SetAttribute("predmet", (*iter).predmet);
			agent->SetAttribute("skript", (*iter).skript.c_str());
    }

    //dump_to_stdout( &doc );
    doc.SaveFile( name.c_str() );
}


int loadscena( SCENA **scena, string name)
{
    int x,y, content, pred, sm;
    string ret;
    char predmet, smer;
	TiXmlDocument doc(name.c_str());
	if (!doc.LoadFile()) { std::cout<< "Nenacitalo sa to"<< std::endl; return 1;}
	TiXmlHandle hDoc(&doc);  //handle na loadnuty dokument

	TiXmlElement* pElem;     //ukazatel na element ktory budeme pouzivat
	TiXmlHandle hRoot(NULL); //pomocny ukazatel na objekty

	pElem=hDoc.FirstChildElement().Element();
	if (!pElem)  { std::cout<< "Nenasiel sa element"<< std::endl; return 1;}
  //ret=pElem->Value();
  //std::cout <<  "NASLI SME ELEMENT:" << ret << std::endl; //dalo by sa tu osetrovat chybu tak ze by sme porovnavali ci
  //je ten prvy element SCENA
  pElem->QueryIntAttribute("x", &x); //pElem nam zatial nejakym sposobom ukazuje na element SCENA
  pElem->QueryIntAttribute("y", &y);
  (*scena) = new SCENA(x, y);

  //std::cout << "Rozmer x sceny:" << x << std::endl;
  //std::cout << "Rozmer y sceny:" << y << std::endl;

		hRoot=TiXmlHandle(pElem); //hRoot sa pohybuje pod SCENOU
		pElem=hRoot.FirstChild( "SURADNICE" ).FirstChild().Element();
		//pElem nam odteraz ukazuje na element SURADNICE
		for(;pElem; pElem=pElem->NextSiblingElement())
		{
			pElem->QueryIntAttribute("x",&x);
			pElem->QueryIntAttribute("y",&y);
			pElem->QueryIntAttribute("content",&content);
			(*scena)->pole[(*scena)->rozmerx*y+x].nova=content;
			//cout<<x<<":"<<y<<" "<<content<<endl;
			//std::cout << "Nacital som x:" << x << std::endl;
			//takto si tu nacitame co potrebujeme...(este treba y a content)
		}
    pElem=hRoot.FirstChild( "AGENTS" ).FirstChild().Element();//presunieme sa s pElem pod AGENTS
		for(; pElem; pElem=pElem->NextSiblingElement())
    {
			pElem->QueryIntAttribute("x",&x);
			pElem->QueryIntAttribute("y",&y);
			pElem->QueryIntAttribute("predmet",&pred);
			pElem->QueryIntAttribute("smer",&sm);
			//std::cout << "Nacital som x agenta:" << x << std::endl;
			ret=pElem->Attribute("skript");
			smer=sm;
			predmet=pred;
			//std::cout << "Nacital som skript agenta:" << ret << std::endl;
			(*scena)->nacitajagenta(AGENT(x,y,smer,predmet, (*scena), ret));
    }
    return 0;
}

/*
int main(int argc, char *argv[])
{
    SCENA scena(15,10);

scena.vlozagenta(AGENT(4,2,'L','-',&scena, "skript.lua"));
scena.vlozagenta(AGENT(5,5,'R','-',&scena, "skript.lua"));
scena.vlozagenta(AGENT(5,4,'U','-',&scena, "skript.lua"));
scena.vlozagenta(AGENT(1,4,'U','-',&scena, "skript.lua"));
scena.vlozagenta(AGENT(2,4,'U','-',&scena, "skript.lua"));
scena.vlozagenta(AGENT(3,4,'U','-',&scena, "skript.lua"));
scena.vlozagenta(AGENT(4,8,'U','-',&scena, "skript.lua"));

 
scena.vlozstenu(3,8);
scena.vlozpredmet(1,7);
scena.vlozpredmet(2,3);
scena.vlozpredmet(2,5);
scena.vlozpredmet(3,2);
scena.vlozpredmet(3,3);
scena.printscena();
    write_simple_doc3( &scena); 
 

    system("PAUSE");
    return EXIT_SUCCESS;
}       
*/
