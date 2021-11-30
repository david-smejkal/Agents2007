/*******************************************************************************
**                          A G E N T S   2 0 0 7                              *
********************************************************************************
**  projekt:      agents2007                                                   *
**  autori:       xsusma00, xsmejk13, xnagye00, xtrnko00                       *
*******************************************************************************/

#include "engine.h"
#include <time.h> 

//Odkaz na agenta vykonavajuceho krok.
AGENT *GlobAgent;


/*
  Funkcia krok zavola na aktualneho agenta jeho nadefinovany skript, ktory
  vykona interpret Lua.
*/
void AGENT::krok()
{
  //printagent();
  GlobAgent = this;
  string str;
  nacitajSkript(str, skript.c_str());
  int s = lua_dostring(L, str.c_str());
  if (s == 0)
  {
    // spusti Lua program
    s = lua_pcall(L, 0, LUA_MULTRET, 0);
  }
}


/*
  Nasledijuce funkcie su registrovane funkcie pre interpret Lua.
  Funkcie menia smer agenta a zaistuju jeho spravne ulozenie
  v scene.
*/

//Obrat agenta dolava.
int turnleft(lua_State *L)
{
  if (GlobAgent->smerpohybu == 'U')
    GlobAgent->smerpohybu = 'L';
  else if (GlobAgent->smerpohybu == 'D')
    GlobAgent->smerpohybu = 'R';
  else if (GlobAgent->smerpohybu == 'R')
    GlobAgent->smerpohybu = 'U';
  else if (GlobAgent->smerpohybu == 'L')
    GlobAgent->smerpohybu = 'D';
  GlobAgent->scena->zmensmer(GlobAgent->posx, GlobAgent->posy, GlobAgent
    ->smerpohybu);
  return 0;
}

//Obrat agenta doprava.
int turnright(lua_State *L)
{
  if (GlobAgent->smerpohybu == 'L')
    GlobAgent->smerpohybu = 'U';
  else if (GlobAgent->smerpohybu == 'R')
    GlobAgent->smerpohybu = 'D';
  else if (GlobAgent->smerpohybu == 'U')
    GlobAgent->smerpohybu = 'R';
  else if (GlobAgent->smerpohybu == 'D')
    GlobAgent->smerpohybu = 'L';
  GlobAgent->scena->zmensmer(GlobAgent->posx, GlobAgent->posy, GlobAgent
    ->smerpohybu);
  return 0;
}

//Obrat agenta dozacu.
int turnback(lua_State *L)
{
  if (GlobAgent->smerpohybu == 'U')
    GlobAgent->smerpohybu = 'D';
  else if (GlobAgent->smerpohybu == 'D')
    GlobAgent->smerpohybu = 'U';
  else if (GlobAgent->smerpohybu == 'R')
    GlobAgent->smerpohybu = 'L';
  else if (GlobAgent->smerpohybu == 'L')
    GlobAgent->smerpohybu = 'R';
  GlobAgent->scena->zmensmer(GlobAgent->posx, GlobAgent->posy, GlobAgent
    ->smerpohybu);
  return 0;
}

//Nahodny obrat agenta.
int turnrandom(lua_State *L)
{
  //srand(time(0));
  switch (rand() % 3)
  {
    case (0): turnright(L);
    break;
    case (1): turnleft(L);
    break;
    case (2): turnback(L);
    break;
  }
  return 0;
}

//Nahodne otocenia agenta s vysokou moznostou, ze sa neotoci.
int canturnrandom(lua_State *L)
{
  //srand(time(0));
  switch (rand() % 10)
  {
    case (0): turnright(L);
    break;
    case (1): turnleft(L);
    break;
    case (2): turnback(L);
    break;
    default:
      break;
  }
  return 0;
}

//Agent zdvihne predmet ak nic nedrzi.
int dvihni(lua_State *L)
{
  if (GlobAgent->predmet != '-')
    return 1;
  char predmetik = GlobAgent->scena->odovzdajpredmet(GlobAgent->posx, GlobAgent
    ->posy);
  GlobAgent->predmet = predmetik;
  return 0;
}

//Agent polozi predmet.
int poloz(lua_State *L)
{
  GlobAgent->scena->prijmipredmet(GlobAgent->posx, GlobAgent->posy); 
  GlobAgent->predmet = '-'; //Agent uz nema predmet.
  return 0;
}

//Agent sa posunie na poziciu pred nim, pokial je to mozne.
int posun(lua_State *L)
{
  int novax = GlobAgent->posx;
  int novay = GlobAgent->posy;
  novapozicia(&novax, &novay, GlobAgent->smerpohybu);
  if (jevolno(L) || (!mampredmet(L) && jepredmet(L)))
  {
    GlobAgent->scena->presunagenta(GlobAgent->posx, GlobAgent->posy, novax,
      novay, GlobAgent->predmet);
    GlobAgent->posx = novax;
    GlobAgent->posy = novay;
  }
  else
    return 1;
  return 0;
}

//Funkcia na zistenie, ci je pred agentom stana.
int jestena(lua_State *L)
{
  int novax = GlobAgent->posx;
  int novay = GlobAgent->posy;
  novapozicia(&novax, &novay, GlobAgent->smerpohybu);
  int pom = GlobAgent->scena->cojetam(novax, novay);
  if (pom == GlobAgent->scena->STENA)
    return 1;
  else
    return 0;
}

//Funkcia na zistenie, ci je vlavo od agenta stana.
int jestenavlavo(lua_State *L)
{
  int novax = GlobAgent->posx;
  int novay = GlobAgent->posy;
  char smer;
  switch (GlobAgent->smerpohybu)
  {
    case ('U'): smer = 'L';
    break;
    case ('D'): smer = 'R';
    break;
    case ('L'): smer = 'D';
    break;
    case ('R'): smer = 'U';
    break;

  }
  novapozicia(&novax, &novay, smer);
  int pom = GlobAgent->scena->cojetam(novax, novay);
  if (pom == GlobAgent->scena->STENA)
    return 1;
  else
    return 0;
}

//Funkcia na zistenie, ci je vpravo od agenta stana.
int jestenavpravo(lua_State *L)
{
  int novax = GlobAgent->posx;
  int novay = GlobAgent->posy;
  char smer;
  switch (GlobAgent->smerpohybu)
  {
    case ('U'): smer = 'R';
    break;
    case ('D'): smer = 'L';
    break;
    case ('L'): smer = 'U';
    break;
    case ('R'): smer = 'D';
    break;
  }
  novapozicia(&novax, &novay, smer);
  int pom = GlobAgent->scena->cojetam(novax, novay);
  if (pom == GlobAgent->scena->STENA)
    return 1;
  else
    return 0;
}

//Funkcia na zistenie, ci je pred agenton volno.
int jevolno(lua_State *L)
{
  int novax = GlobAgent->posx;
  int novay = GlobAgent->posy;
  novapozicia(&novax, &novay, GlobAgent->smerpohybu);
  int pom = GlobAgent->scena->cojetam(novax, novay);
  if (pom == GlobAgent->scena->VOLNO)
    return 1;
  else
    return 0;
}

//Funkcia na zistenie, ci je pred agentom predmet.
int jepredmet(lua_State *L)
{
  int novax = GlobAgent->posx;
  int novay = GlobAgent->posy;
  novapozicia(&novax, &novay, GlobAgent->smerpohybu);
  int pom = GlobAgent->scena->cojetam(novax, novay);
  if (pom == GlobAgent->scena->PD)
    return 1;
  else
    return 0;
}

//Funkcia zisti, ci agent drzi predmet.
int mampredmet(lua_State *L)
{
  if (GlobAgent->predmet != '-')
    return 1;
  else
    return 0;
}

//Funkcia na zistenie, ci je pred agentom iny agent.
int jeagent(lua_State *L)
{
  int novax = GlobAgent->posx;
  int novay = GlobAgent->posy;
  novapozicia(&novax, &novay, GlobAgent->smerpohybu);
  int pom = GlobAgent->scena->cojetam(novax, novay);
  if (pom >= GlobAgent->scena->AU && pom <= GlobAgent->scena->ALPH)
    return 1;
  else
    return 0;
}

//Pomovna funkcia na vypocet nasledujucej pozicie agenta.
void novapozicia(int *novax, int *novay, char smer)
{
  switch (smer)
  {
    case ('U'): (*novay)--;
    break;
    case ('L'): (*novax)--;
    break;
    case ('R'): (*novax)++;
    break;
    case ('D'): (*novay)++;
    break;
  }
}


/*
  Funkcia na vykreslenie aktualnej sceny v ASCII.
  (Pomocna funkcia pri vyvoji aplikacie)
*/
void SCENA::printscena()
{
  cout << "vykreslenie sceny, kolo:" << pocetkol << endl;
  int i, j, k;
  for (j = 0; j < rozmery; j++)
  {
    cout << "-";
    for (k = 0; k < rozmerx; k++)
    {
      cout << "-----";
    }
    cout << endl;
    for (i = 0; i < rozmerx; i++)
    {
      cout << "|";
      switch (pole[rozmerx *j + i].nova)
      {
        case (VOLNO): cout << "    ";
        break;
        case (STENA): cout << "xxxx";
        break;
        case (PD): cout << "PD  ";
        break;
        case (AU): cout << "AU  ";
        break;
        case (AD): cout << "AD  ";
        break;
        case (AR): cout << "AR  ";
        break;
        case (AL): cout << "AL  ";
        break;
        case (AUPD): cout << "AUPD";
        break;
        case (ADPD): cout << "ADPD";
        break;
        case (ARPD): cout << "ARPD";
        break;
        case (ALPD): cout << "ALPD";
        break;
        case (AUPH): cout << "AUPH";
        break;
        case (ADPH): cout << "ADPH";
        break;
        case (ARPH): cout << "ARPH";
        break;
        case (ALPH): cout << "ALPH";
        break;
      }
    }
    cout << "|" << endl;
  }
  cout << "-";
  for (k = 0; k < rozmerx; k++)
  {
    cout << "-----";
  }
  cout << endl;
}

/*
  Funkcia na vykreslenie predoslej sceny v ASCII.
  (Pomocna funkcia pri vyvoji aplikacie)
*/
void SCENA::printscenastara()
{
  cout << "vykreslenie sceny, kolo:" << pocetkol << endl;
  int i, j, k;
  for (j = 0; j < rozmery; j++)
  {
    cout << "-";
    for (k = 0; k < rozmerx; k++)
    {
      cout << "-----";
    }
    cout << endl;
    for (i = 0; i < rozmerx; i++)
    {
      cout << "|";
      switch (pole[rozmerx *j + i].stara)
      {
        case (VOLNO): cout << "    ";
        break;
        case (STENA): cout << "::::";
        break;
        case (PD): cout << "PD  ";
        break;
        case (AU): cout << "AU  ";
        break;
        case (AD): cout << "AD  ";
        break;
        case (AR): cout << "AR  ";
        break;
        case (AL): cout << "AL  ";
        break;
        case (AUPD): cout << "AUPD";
        break;
        case (ADPD): cout << "ADPD";
        break;
        case (ARPD): cout << "ARPD";
        break;
        case (ALPD): cout << "ALPD";
        break;
        case (AUPH): cout << "AUPH";
        break;
        case (ADPH): cout << "ADPH";
        break;
        case (ARPH): cout << "ARPH";
        break;
        case (ALPH): cout << "ALPH";
        break;
      }
    }
    cout << "|" << endl;
  }
  cout << "-";
  for (k = 0; k < rozmerx; k++)
  {
    cout << "-----";
  }
  cout << endl;
}

//Funkcia vracia aktualny obsah pozicie x,y.
int SCENA::cojetam(int x, int y)
{
  return pole[rozmerx *y + x].nova;
}

//Funkcia vracia predosli obsah pozicie x,y.
int SCENA::cotambolo(int x, int y)
{
  return pole[rozmerx *y + x].stara;
}

/*
  Funkcia vlozi agenta do sceny, pokial je na pozicii volno. Zapise udaje do
  agentovy do aktualnej sceny, a prida ho do zoznamu agentov.
*/
int SCENA::vlozagenta(AGENT agent)
{
  int x = agent.vratpoziciux();
  int y = agent.vratpoziciuy();
  char smer = agent.vratsmer();
  if (x > rozmerx - 1)
  {
    cerr << "nie je mozne vlozit agenta mimo plochy!" << endl;
    return 1;
  }
  if (y > rozmery - 1)
  {
    cerr << "nie je mozne vlozit agenta mimo plochy!" << endl;
    return 1;
  }
  int cotam = cojetam(x, y);
  if (cotam != VOLNO)
  {
    cerr << "nie je mozne vlozit agenta! nieco tam uz je!" << endl;
    return 1;
  }
  switch (smer)
  {
    case ('U'): pole[rozmerx *y + x].nova = AU;
    break;
    case ('D'): pole[rozmerx *y + x].nova = AD;
    break;
    case ('R'): pole[rozmerx *y + x].nova = AR;
    break;
    case ('L'): pole[rozmerx *y + x].nova = AL;
    break;
  }
  agents.push_back(agent);
  return 0;
}

/*
  Funkcia vlozi agenta do zoznamu agentov.
*/
int SCENA::nacitajagenta(AGENT agent)
{
  int x = agent.vratpoziciux();
  int y = agent.vratpoziciuy();
  if (x > rozmerx - 1)
  {
    cerr << "nie je mozne vlozit agenta mimo plochy!" << endl;
    return 1;
  }
  if (y > rozmery - 1)
  {
    cerr << "nie je mozne vlozit agenta mimo plochy!" << endl;
    return 1;
  }
  agents.push_back(agent);
  return 0;
}

/*
  Funkcia uvolni poziciu [x, y] a pokial tam bol agent, odstrani ho zo zoznamu
  agentov. Nieje mozne odstranit obvodovu stenu!
*/
void SCENA::zmazpoziciu(int x, int y)
{
  //cout<<"mam zmazat pos:" << x<<":"<<y<<endl;
  if (x <= 0 || y <= 0 || x >= (rozmerx-1) || y >= (rozmery-1))
    return;
  //cout<<"mazem ju!"<<endl;
  pole[rozmerx *y + x].nova = VOLNO;
  for (IterAgents iter = agents.begin(); iter != agents.end(); iter++)
  {
    if ((*iter).posx == x && (*iter).posy == y)
    {
      //Mazem agenta zo zoznamu agentov.
      iter = agents.erase(iter);
    }
  }
}

//Funkcia vlozi na scenu predmet, pokial je pozicia [x, y] volna.
int SCENA::vlozpredmet(int x, int y)
{
  if (x > rozmerx - 1)
  {
    cerr << "nie je mozne vlozit predmet mimo plochy!" << endl;
    return 1;
  }
  if (y > rozmery - 1)
  {
    cerr << "nie je mozne vlozit predmet mimo plochy!" << endl;
    return 1;
  }
  int cotam = cojetam(x, y);
  if (cotam != VOLNO)
  {
    cerr << "nie je mozne vlozit predmet! nieco tam uz je!" << endl;
    return 1;
  }
  pole[rozmerx *y + x].nova = PD;
  return 0;
}

//Funkcia vlozi na scenu stenu, pokial je pozicia [x, y] volna.
int SCENA::vlozstenu(int x, int y)
{
  if (x > rozmerx - 1)
  {
    cerr << "nie je mozne vlozit stenu mimo plochy!" << endl;
    return 1;
  }
  if (y > rozmery - 1)
  {
    cerr << "nie je mozne vlozit stenu mimo plochy!" << endl;
    return 1;
  }
  int cotam = cojetam(x, y);
  if (cotam != VOLNO)
  {
    cerr << "nie je mozne vlozit stenu! nieco tam uz je!" << endl;
    return 1;
  }
  pole[rozmerx *y + x].nova = STENA;
  return 0;
}


/*
  Funkcia zmeni poziciu agenta na scena a prepise aj udaje agentovi.
*/
void SCENA::presunagenta(int odkialx, int odkialy, int kamx, int kamy, char
  predmet)
{
  int predosle = pole[rozmerx *odkialy + odkialx].nova;
  int buduce = pole[rozmerx *kamy + kamx].nova;
  
  //Nova pozicia nemoze byt obsadena agentom ani stenou.
  if ((buduce >= AU && buduce <= ALPH) || buduce == STENA)
    return ;
  
  //Ak agent ide sam bez predmetu.
  if (predmet == '-')
  {
    //Ak na predoslom policku bol sam agent.
    if (predosle >= AU && predosle <= AL)
    {
      //Predosle policko bude volne.
      pole[rozmerx *odkialy + odkialx].nova = VOLNO; 
    }
    //Ak na predoslom policku bol agent aj predmet, ale agent ho nedrzi.
    else if (predosle >= AUPD && predosle <= ALPD)
    {
      //Po agentovom odchode na pozicii zostava predmet.
      pole[rozmerx *odkialy + odkialx].nova = PD;
    }
    //Ak agent ide na volnu poziciu.
    if (pole[rozmerx *kamy + kamx].nova == VOLNO)
    {
      //Na novu poziciu sa ulozi agent bez predmetu.
      if (predosle >= AUPD && predosle <= ALPD)
        //Ked bol agent na policku s predmetom, (-4) zaisti, ze teraz pri nom
        //predmet nebude a agent si zachva spravny smer. (Viz ENUM obsah...)
        pole[rozmerx *kamy + kamx].nova = predosle - 4;
      else
        pole[rozmerx *kamy + kamx].nova = predosle;
    }
    //Ak agent ide na poziciu kde je predmet.
    else
    {
      //Ked bol agent na policku bez predmetom, (+4) zaisti, ze teraz pri nom
      //predmet bude a agent si zachva spravny smer. (Viz ENUM obsah...)
      if (predosle >= AUPD && predosle <= ALPD)
        pole[rozmerx *kamy + kamx].nova = predosle;
      else
        pole[rozmerx *kamy + kamx].nova = predosle + 4;
    }
  }
  //Agent drzi predmet.
  else
  {
    pole[rozmerx *kamy + kamx].nova = predosle;
    pole[rozmerx *odkialy + odkialx].nova = VOLNO;
  }
}

//Funkcia meni smer agenta v scene.
void SCENA::zmensmer(int x, int y, char novysmer)
{
  if (pole[rozmerx *y + x].nova >= AUPD && pole[rozmerx *y + x].nova <= ALPD)
  {
    switch (novysmer)
    {
      case ('U'): pole[rozmerx *y + x].nova = AUPD;
      break;
      case ('D'): pole[rozmerx *y + x].nova = ADPD;
      break;
      case ('L'): pole[rozmerx *y + x].nova = ALPD;
      break;
      case ('R'): pole[rozmerx *y + x].nova = ARPD;
      break;
    }
  }
  else if (pole[rozmerx *y + x].nova >= AU && pole[rozmerx *y + x].nova <= AL)
  {
    switch (novysmer)
    {
      case ('U'): pole[rozmerx *y + x].nova = AU;
      break;
      case ('D'): pole[rozmerx *y + x].nova = AD;
      break;
      case ('L'): pole[rozmerx *y + x].nova = AL;
      break;
      case ('R'): pole[rozmerx *y + x].nova = AR;
      break;
    }
  }
  else if (pole[rozmerx *y + x].nova >= AUPH && pole[rozmerx *y + x].nova <=
    ALPH)
  {
    switch (novysmer)
    {
      case ('U'): pole[rozmerx *y + x].nova = AUPH;
      break;
      case ('D'): pole[rozmerx *y + x].nova = ADPH;
      break;
      case ('L'): pole[rozmerx *y + x].nova = ALPH;
      break;
      case ('R'): pole[rozmerx *y + x].nova = ARPH;
      break;
    }
  }
}

//Funkcia meni smer agenta v scene v smere hod o 1.
void SCENA::otocojedno(int x, int y)
{
  if(x > rozmerx -1 || y > rozmery -1) return;

  //char smeragenta;
  //IterAgents iter_pom;
  for (IterAgents iter = agents.begin(); iter != agents.end(); iter++)
  {
    //Agent vykona krok.
    if((*iter).vratpoziciux() == x && (*iter).vratpoziciuy() == y)
    {
        //smeragenta = (*iter).smerpohybu;
        //cout << "pred\n";
        GlobAgent = &(*iter);
        turnright(GlobAgent->L);
        return;
    }
  }
  
  /*if (pole[rozmerx *x + y].nova >= AUPD && pole[rozmerx *x + y].nova <= ALPD)
  {
    switch ((*iter_pom).smerpohybu)
    {
      case ('U'): pole[rozmerx *x + y].nova = ARPD; (*iter_pom).smerpohybu = 'R';
      break;
      case ('D'): pole[rozmerx *x + y].nova = ALPD; (*iter_pom).smerpohybu = 'L';
      break;
      case ('L'): pole[rozmerx *x + y].nova = AUPD; (*iter_pom).smerpohybu = 'U';
      break;
      case ('R'): pole[rozmerx *x + y].nova = ADPD; (*iter_pom).smerpohybu = 'D';
      break;
    }cout << "switch\n";
  }
  else if (pole[rozmerx *x + y].nova >= AU && pole[rozmerx *x + y].nova <= AL)
  {
    switch ((*iter_pom).smerpohybu)
    {
      case ('U'): pole[rozmerx *x + y].nova = AR; (*iter_pom).smerpohybu = 'R';
      break;
      case ('D'): pole[rozmerx *x + y].nova = AL; (*iter_pom).smerpohybu = 'L';
      break;
      case ('L'): pole[rozmerx *x + y].nova = AU; (*iter_pom).smerpohybu = 'U';
      break;
      case ('R'): pole[rozmerx *x + y].nova = AD; (*iter_pom).smerpohybu = 'D';
      break;
    }
  }
  else if (pole[rozmerx *x + y].nova >= AUPH && pole[rozmerx *x + y].nova <=
    ALPH)
  {
    switch ((*iter_pom).smerpohybu)
    {
      case ('U'): pole[rozmerx *x + y].nova = ARPH; (*iter_pom).smerpohybu = 'R';
      break;
      case ('D'): pole[rozmerx *x + y].nova = ALPH; (*iter_pom).smerpohybu = 'L';
      break;
      case ('L'): pole[rozmerx *x + y].nova = AUPH; (*iter_pom).smerpohybu = 'U';
      break;
      case ('R'): pole[rozmerx *x + y].nova = ADPH; (*iter_pom).smerpohybu = 'D';
      break;
    }
  }*/
}

/*
  Agent dvihne predmet zo sceny.
*/
char SCENA::odovzdajpredmet(int x, int y)
{
  //Pokial je na pozicii predmet aj agent.
  if (pole[rozmerx *y + x].nova >= AUPD)
  {
    switch (pole[rozmerx *y + x].nova)
    {
      case (AUPD): pole[rozmerx *y + x].nova = AUPH;
      break;
      case (ADPD): pole[rozmerx *y + x].nova = ADPH;
      break;
      case (ALPD): pole[rozmerx *y + x].nova = ALPH;
      break;
      case (ARPD): pole[rozmerx *y + x].nova = ARPH;
      break;
    }
    return 'P'; //Vrati predmet ktori agent zdvihol.
  }
  //Ak by tam nebol ziaden predmet a je snaha ho dvihnut, nic sa nedvihne.
  else
    return '-';
}

/*
  Agent polozi predmet na scenu.
*/
void SCENA::prijmipredmet(int x, int y)
{
  //Prepiseme poziciu v scene.
  switch (pole[rozmerx *y + x].nova)
  {
    case (AUPH): pole[rozmerx *y + x].nova = AUPD;
    break;
    case (ADPH): pole[rozmerx *y + x].nova = ADPD;
    break;
    case (ALPH): pole[rozmerx *y + x].nova = ALPD;
    break;
    case (ARPH): pole[rozmerx *y + x].nova = ARPD;
    break;
  }
}

/*
  Funkcia odzalohuje scenu.
*/
void SCENA::odlozscenu()
{
  for (int i = 0; i < rozmery; i++)
  {
    for (int j = 0; j < rozmerx; j++)
    {
      pole[rozmerx *i + j].stara = pole[rozmerx *i + j].nova;
    }
  }
}

/*
  Scena prechadza zoznam agentov a vola ich postupne, aby robili kroky podla
  vlastneho skriptu.
*/
void SCENA::kolo()
{
  //Pripocita jedno kolo
  pocetkol++;

  for (IterAgents iter = agents.begin(); iter != agents.end(); iter++)
  {
    //Agent vykona krok.
    (*iter).krok();
  }
    //printscenastara();
    //printscena();
}



/*
int main(int argc, char *argv[])
{
SCENA scena(15,10);

scena.vlozagenta(AGENT(4,2,'L','-',&scena, "skript.xml"));
scena.vlozagenta(AGENT(5,5,'R','-',&scena, "skript.xml"));
scena.vlozagenta(AGENT(5,4,'U','-',&scena, "skript.xml"));
scena.vlozagenta(AGENT(1,4,'U','-',&scena, "skript.xml"));
scena.vlozagenta(AGENT(2,4,'U','-',&scena, "skript.xml"));
scena.vlozagenta(AGENT(3,4,'U','-',&scena, "skript.xml"));
scena.vlozagenta(AGENT(4,8,'U','-',&scena, "skript.xml"));


scena.vlozstenu(3,7);
scena.vlozstenu(3,8);
scena.vlozpredmet(1,7);
scena.vlozpredmet(2,3);
scena.vlozpredmet(2,5);
scena.vlozpredmet(3,2);
scena.vlozpredmet(3,3);

//savescena( &scena); 

//scena.printscena();
SCENA *scena;
loadscena(&scena);

string prikaz;
(*scena).printscena();
//scena.printscenastara();
(*scena).zmazpoziciu(1,4);

(*scena).printscena();
while (cin>>prikaz)
{
(*scena).odlozscenu();
if (prikaz=="x") break;
if (prikaz=="s") savescena( scena);
(*scena).kolo();
Sleep(300);
(*scena).printscena();
//scena.printscenastara();
}
(*scena).~SCENA();
system("PAUSE");
return EXIT_SUCCESS;
}
 */
