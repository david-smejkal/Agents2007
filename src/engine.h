/*******************************************************************************
**                          A G E N T S   2 0 0 7                              *
********************************************************************************
**  projekt:      agents2007                                                   *
**  autori:       xsusma00, xsmejk13, xnagye00, xtrnko00                       *
*******************************************************************************/

#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>

extern "C"
{
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

using namespace std;

class SCENA;
class AGENT;

/*
  Funkcie na pracu so subormi XML.
  Su definovane v "xml.cpp".
*/
void savescena(SCENA *scena, string name);
int nacitajSkript(string &ret, string subor);
int ulozSkript(string &skript, string subor);
int loadscena(SCENA **scena, string name);

/*
  Funkcie na ovladanie agentov a zistovani udojov o agentoch a ich okoli.
  Funkcie budu registrovane interpreton Lua a su definovane v "engine.cpp".
*/
int turnleft(lua_State *L);
int turnright(lua_State *L);
int turnback(lua_State *L);
int turnrandom(lua_State *L);
int canturnrandom(lua_State *L);
int gorandom(lua_State *L);
int dvihni(lua_State *L);
int poloz(lua_State *L);
int posun(lua_State *L);
int jestena(lua_State *L);
int jestenavlavo(lua_State *L);
int jestenavpravo(lua_State *L);
int jevolno(lua_State *L);
int jepredmet(lua_State *L);
int mampredmet(lua_State *L);
int jeagent(lua_State *L);
void novapozicia(int *novax, int *novay, char smer);

/*
  Class AGENT predstavuje agenta a obsahuje jeho udaje.
*/
class AGENT
{
  public:
    int posx, posy;   //Pozicia agenta v scene.
    char smerpohybu;  //Udaj o smere agenta: U,D,R,L up down right left
    char predmet;     //Predmet ktory drzi agent, ('-' agent predmet nema).
    SCENA *scena;     //Scena na ktorej sa agent nachadza.
    string skript;    //Meno skriptu v XML.
    
    void krok(); 

  public:
    lua_State *L;

    //cons, dest
    AGENT(int x, int y, char smer, char teleso, SCENA *Scena, string Skript)
    {
        scena = Scena;
        skript = Skript;
        posx = x;
        posy = y;
        predmet = teleso;
        smerpohybu = smer;

        L = lua_open();
        luaopen_io(L);
        luaopen_base(L);
        luaopen_table(L);
        luaopen_string(L);
        luaopen_math(L);
        luaopen_loadlib(L);
        
        //Registracie funkcii interpreton Lua.
        lua_register(L, "jevolno", jevolno);
        lua_register(L, "turnleft", turnleft);
        lua_register(L, "turnright", turnright);
        lua_register(L, "turnback", turnback);
        lua_register(L, "turnrandom", turnrandom);
        lua_register(L, "canturnrandom", canturnrandom);
        lua_register(L, "dvihni", dvihni);
        lua_register(L, "poloz", poloz);
        lua_register(L, "posun", posun);
        lua_register(L, "jestena", jestena);
        lua_register(L, "jestenavlavo", jestenavlavo);
        lua_register(L, "jestenavpravo", jestenavpravo);
        lua_register(L, "jevolno", jevolno);
        lua_register(L, "jepredmet", jepredmet);
        lua_register(L, "mampredmet", mampredmet);
        lua_register(L, "jeagent", jeagent);
    }
    
    void printagent()
    { //Vypis agenta (pomocna funkcia pre vyvoj).
      cout << "pozicia[" << posx << "," << posy << "], smer[" << smerpohybu
        << "], predmet[" << predmet << "]" << endl;
    }
    int vratpoziciux()
    { //Vrati poziciu x agenta.
      return posx;
    }
    int vratpoziciuy()
    { //Vrati poziciu y agenta.
      return posy;
    }
    int vratsmer()
    { //Vrati smer agenta.
      return smerpohybu;
    }
};


/*
  Vytvorenie typu Agents a typu IterAgents, iteratoru nan.
  Agents je zoznam obsahujuci prvky typu AGENT.
*/
typedef list < AGENT > Agents;
typedef Agents::iterator IterAgents;

//Pomocna struktura, vyuzije sa v scene, kde je prvkom pola.
struct SPozicia
{
  int stara;
  int nova;
};

/*
  Class SCENA predstavuje scenu a udaje o nej.
*/
class SCENA
{
  public:
  long pocetkol; //Pocitadlo kol.

  public:
    SPozicia *pole; //Jednorozmerne pole
    Agents agents; //Zoznam agentov.
    int rozmerx, rozmery; //Rozmery sceny x, y.

    //cons, dest
    SCENA(int x, int y)
    {
      pocetkol = 0;
      pole = new SPozicia[x *y];
      rozmerx = x;
      rozmery = y;
      int i, j;
      for (j = 0; j < y; j++)
      {
        for (i = 0; i < x; i++)
        {
          pole[rozmerx *j + i].stara = VOLNO;
          if (j == 0 || i == 0 || j == y - 1 || i == x - 1)
          { //Ohranicenie sceny stenou.
            pole[rozmerx *j + i].nova = STENA;
          
          }
          else
            //Na inych poziciach je volno.
            pole[rozmerx *j + i].nova = VOLNO;
          //cout<<", pos: "<<rozmerx *j + i;   
        
        }
        //cout<<endl;
      }
    }
    ~SCENA()
    {
      delete pole;
      agents.clear();
    }

    //Funkcie su definovane v "engine.cpp"
    void odlozscenu();
    void kolo();
    void printscena();
    void printscenastara();
    int cojetam(int x, int y);
    int cotambolo(int x, int y);
    void zmazpoziciu(int x, int y);
    int vlozstenu(int x, int y);
    int vlozagenta(AGENT agent);
    int vlozpredmet(int x, int y);
    int nacitajagenta(AGENT agent);
    void presunagenta(int odkialx, int odkialy, int kamx, int kamy,
      char predmet);
    void zmensmer(int x, int y, char novysmer);
    void otocojedno(int x, int y);
    char odovzdajpredmet(int x, int y);
    void prijmipredmet(int x, int y);

    enum obsah
    {
      VOLNO, STENA, PD,
      AU, AD, AR, AL, //Agent bez predmetu.
      AUPD, ADPD, ARPD, ALPD, //Agent a predmet na zemi.
      AUPH, ADPH, ARPH, ALPH, //Agent s predmetom.
    };
};

enum obsahs
{
  VOLNO, STENA, PD,
  AU, AD, AR, AL, //Agent bez predmetu.
  AUPD, ADPD, ARPD, ALPD, //Agent a predmet na zemi.
  AUPH, ADPH, ARPH, ALPH, //Agent s predmetom.

};

#endif
