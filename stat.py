#!/usr/bin/python
# AGENTS 2007 - skript pre predmet IPP
# autori:
# xnagye00
# xtrnko00
# xsusma00
# xsmejk13

import sys    
import re   
import os

zoznamik={}
subory=os.listdir('src')

#nadefinujem si klucove slova, snaha bola podla abecedy, ale potom aj podla toho,
# aby ked je X podmnozinou Y, tak Y je v zozname SKOR(aby sa neopakoval vyskyt)
# pomocou sortu to nesplnalo ucel:(
klucoveslova=["asm", "auto", "bool", "break", "case", "catch", "char",\
        "class", "const_cast", "const", "continue", "default", "delete",\
        "double", "dynamic_cast", "else", "enum", "explicit",\
        "export", "extern", "false", "float", "for", "friend", "goto",\
        "if", "inline", "int", "long", "mutable", "namespace", "new",\
        "operator", "private", "protected", "public", "register",\
        "reinterpret_cast", "return", "short", "signed", "sizeof",\
        "static_cast", "static", "struct", "switch", "template", "this",\
        "throw", "true", "try", "typedef", "typeid", "typename", "union",\
        "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while",\
        "bitand", "compl", "not_eq", "or_eq", "xor_eq",\
        "and_eq", "bitor", "not", "xor", "or", "and", "do"]

#vypis vsetkych operatorov
operatory=["dynamic_cast", "static_cast", "reinterpret_cast", "const_cast",\
         "typeid","sizeof",\
         "<<=", ">>=","->*", "*=", "/=", "%=", "+=", "-=", "&=", "^=", "!=", "<=",\
         ">=", "==", "!=", "->", ".*", "<<", ">>", "||", "&&", "--", "++",\
         "::", ".", ",", "=", "|", "^", "&", ">", "<", "+", "-", "*", "/", "%",\
         "!", "~","(", "[",")", "]","?"]

#funkcia pre vypis poctu klucovych slov
def k():
  zoznamik.clear() #vyprazdni zoznamik ak by v nom nieco nahodou bolo
  celkovo=0
  vaktualnomsubore=0 #nastavim si vsetky pocitadla na nulu
  for aktualnysubor in subory: #prehladavam medzi subormi
    if (re.match(".*\.cpp$|.*\.h$",aktualnysubor)):
      #otvorim si vsetky subory s priponou .cpp .c alebo .h
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      infile=file(aktualnysubor,'r')
      data = infile.read()
      infile.close()
      #odstranime komentare
      data=odstrankomentar(data)
      #hladam v klucovych slovach
      for slovo in klucoveslova:
        najdi=re.findall('\W'+slovo+'\W',data)
        #co naslo, nahradim medzerou
        data=re.sub(slovo,' ',data)
        najdene=str(najdi)
        celkovo+=najdene.count(slovo)
        vaktualnomsubore+=najdene.count(slovo)
      #zistim si cestu k suboru
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      #priradim do asociativneho pola
      zoznamik[aktualnysubor]=vaktualnomsubore
      vaktualnomsubore=0
  #cyklus pre vypisovanie a zarovnavanie
  for aktualnysubor in zoznamik:
    retazec=aktualnysubor
    cislo=str(zoznamik[aktualnysubor])
    print retazec.ljust(najdlhsinazov(zoznamik)), cislo.rjust(najdlhsiecislo(zoznamik))
  strcelkovo=str(celkovo)
  #vypis vsetkeho dokopy
  print 'CELKEM:'.ljust(najdlhsinazov(zoznamik)), strcelkovo.rjust(najdlhsiecislo(zoznamik))
  
#funkcia pre vypis poctu operatorov
def o():
  zoznamik.clear()
  celkovo=0
  vaktualnomsubore=0
  for aktualnysubor in subory:
    if (re.match(".*\.cpp$|.*\.h$|.*\.c$",aktualnysubor)):
      #otvorim si vsetky subory s priponou .cpp .c alebo .h
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      infile=file(aktualnysubor,'r')
      data = infile.read()
      infile.close()
      #odstranime komentare a () ak medzi nimi nic nie je
      data=odstrankomentar(data)
      data=data.replace('()',' ')
      #spocitam operatory
      for slovo in operatory:
        celkovo+=data.count(slovo)
        vaktualnomsubore+=data.count(slovo)
        data=data.replace(slovo,' ')
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      zoznamik[aktualnysubor]=vaktualnomsubore
      vaktualnomsubore=0
  #nasleduje zarovnany vypis
  for aktualnysubor in zoznamik:
    retazec=aktualnysubor
    cislo=str(zoznamik[aktualnysubor])
    print retazec.ljust(najdlhsinazov(zoznamik)), cislo.rjust(najdlhsiecislo(zoznamik))
  strcelkovo=str(celkovo)
  print 'CELKEM:'.ljust(najdlhsinazov(zoznamik)), strcelkovo.rjust(najdlhsiecislo(zoznamik))

#funkcia pre vypis poctu IDENTIFIKATOROV
def i():
  zoznamik.clear() #vyprazdni zoznamik ak by v nom nieco nahodou bolo
  celkovo=0
  vaktualnomsubore=0 #nastavim si vsetky pocitadla na nulu
  for aktualnysubor in subory: #prehladavam medzi subormi
    if (re.match(".*\.cpp$|.*\.h$|.*\.c$",aktualnysubor)):
      #otvorim si vsetky subory s priponou .cpp .c alebo .h
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      infile=file(aktualnysubor,'r')
      data = infile.read()
      infile.close()
      #odstranime komentare
      data=odstrankomentar(data)
      #spocitam identifikatory
      p = re.compile('[A-Za-z_][A-Za-z_0-9]*')
      vysledok = p.findall(data)
      celkovo+=len(vysledok)
      vaktualnomsubore+=len(vysledok)
      #zistim si cestu k suboru
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      #priradim do asociativneho pola
      zoznamik[aktualnysubor]=vaktualnomsubore
      vaktualnomsubore=0
  #cyklus pre vypisovanie a zarovnavanie
  for aktualnysubor in zoznamik:
    retazec=aktualnysubor
    cislo=str(zoznamik[aktualnysubor])
    print retazec.ljust(najdlhsinazov(zoznamik)), cislo.rjust(najdlhsiecislo(zoznamik))
  strcelkovo=str(celkovo)
  #vypis vsetkeho dokopy
  print 'CELKEM:'.ljust(najdlhsinazov(zoznamik)), strcelkovo.rjust(najdlhsiecislo(zoznamik))

#funkcia pre vypis poctu IDENTIFIKATOROV bez KLUCOVYCH slov
def ik():
  zoznamik.clear() #vyprazdni zoznamik ak by v nom nieco nahodou bolo
  celkovo=0
  vaktualnomsubore=0 #nastavim si vsetky pocitadla na nulu
  for aktualnysubor in subory: #prehladavam medzi subormi
    if (re.match(".*\.cpp$|.*\.h$|.*\.c$",aktualnysubor)):
      #otvorim si vsetky subory s priponou .cpp .c alebo .h
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      infile=file(aktualnysubor,'r')
      data = infile.read()
      infile.close()
      #odstranime komentare
      data=odstrankomentar(data)
      #spocitam vsety identifikatory
      p = re.compile('[A-Za-z_][A-Za-z_0-9]*')
      pocet = p.findall(data)
      celkovo+=len(pocet)
      vaktualnomsubore+=len(pocet)
      #odcitam klucove slova
      for slovo in klucoveslova:
        najdi=re.findall('\W'+slovo+'\W',data)
        #co naslo, nahradim medzerou
        data=re.sub(slovo,' ',data)
        najdene=str(najdi)
        celkovo-=najdene.count(slovo)
        vaktualnomsubore-=najdene.count(slovo)      
      #zistim si cestu k suboru
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      #priradim do asociativneho pola
      zoznamik[aktualnysubor]=vaktualnomsubore
      vaktualnomsubore=0
  #cyklus pre vypisovanie a zarovnavanie
  for aktualnysubor in zoznamik:
    retazec=aktualnysubor
    cislo=str(zoznamik[aktualnysubor])
    print retazec.ljust(najdlhsinazov(zoznamik)), cislo.rjust(najdlhsiecislo(zoznamik))
  strcelkovo=str(celkovo)
  #vypis vsetkeho dokopy
  print 'CELKEM:'.ljust(najdlhsinazov(zoznamik)), strcelkovo.rjust(najdlhsiecislo(zoznamik))

#funkcia pre vypis poctu slova PATTERN
def w(pattern):
  zoznamik.clear() #vyprazdni zoznamik ak by v nom nieco nahodou bolo
  celkovo=0
  vaktualnomsubore=0 #nastavim si vsetky pocitadla na nulu
  for aktualnysubor in subory: #prehladavam medzi subormi
    if (re.match(".*\.cpp$|.*\.h$|.*\.c$",aktualnysubor)):
      #otvorim si vsetky subory s priponou .cpp .c alebo .h
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      infile=file(aktualnysubor,'r')
      data = infile.read()
      infile.close()
      celkovo+=data.count(pattern)
      vaktualnomsubore+=data.count(pattern)
      data=data.replace(pattern,' ')
      #zistim si cestu k suboru
      cesticka=cesta()
      aktualnysubor=os.path.join(cesticka, aktualnysubor)
      #priradim do asociativneho pola
      zoznamik[aktualnysubor]=vaktualnomsubore
      vaktualnomsubore=0
  #cyklus pre vypisovanie a zarovnavanie
  for aktualnysubor in zoznamik:
    retazec=aktualnysubor
    cislo=str(zoznamik[aktualnysubor])
    print retazec.ljust(najdlhsinazov(zoznamik)), cislo.rjust(najdlhsiecislo(zoznamik))
  strcelkovo=str(celkovo)
  #vypis vsetkeho dokopy
  print 'CELKEM:'.ljust(najdlhsinazov(zoznamik)), strcelkovo.rjust(najdlhsiecislo(zoznamik))

#napoveda
def help():
  print '--help vypise tuto napovedu'
  print '-k     vypise pocet KLUCOVYCH SLOV slov v kazdom zdrojovom subore a celkom'
  print '-o     vypise pocet OPERATOROV v kazdom zdrojovom subore a celkom'
  print '-i     vypise pocet IDENTIFIKATOROV v kazdom zdrojovom subore a celkom'
  print '-ik    vypise pocet IDENTIFIKATOROV v kazdom zdrojovom subore a celkom(nezahrnuje klucove slova)'
  print '-w <pattern> vypise pocet vyskytu slova pattern v kazdom zdrojovom subore a celkom'

#funkcia, ktora vrati cestu k suboru
def cesta():
  return os.path.abspath(os.curdir)+'/src/'

#funkcia, ktora vrati dlzku najdlhsieho nazvu suboru, aby to mohlo zarovnavat
def najdlhsinazov(zoznam):
  najdlhsi=0
  for aktualnysubor in zoznamik:
    aktualnadlzka=len(aktualnysubor)
    if (aktualnadlzka > najdlhsi):
      najdlhsi=aktualnadlzka
  return najdlhsi+3

#funkcia, ktora vrati dlzku najvyssieho cisla, aby to mohlo zarovnavat
def najdlhsiecislo(zoznam):
  najdlhsi=0
  for aktualnysubor in zoznamik:
    cislo=str(zoznamik[aktualnysubor])
    aktualnadlzka=len(cislo)
    if (aktualnadlzka > najdlhsi):
      najdlhsi=aktualnadlzka
  return najdlhsi+2

#funkcia, ktora odstranuje komentar a konce riadkov
def odstrankomentar(text):
      text=re.sub('#include.*',' ',text) #najskor odstranim includy
      text=re.sub('#define.*',' ',text) #najskor potom define
      text=re.sub('//.*',' ',text) #najskor odstranim komentar, ktory je ku koncu riadku, nahradim medzerou
      text=re.sub('\n',' ' ,text) #potom odstranim konce riadkov
      text=re.sub('/\*.*?\*/',' ',text) #odstranim vsetky komentare medzi hviezdickami
      return text                #vratim zmeneny text


if sys.argv[1]=="--help":
  help()
elif sys.argv[1]=="-k":
  k()
elif sys.argv[1]=="-o":
  o()
elif sys.argv[1]=="-i":
  i()
elif sys.argv[1]=="-ik":
  ik()
elif sys.argv[1]=="-w":
  if sys.argv[2]=="":
    print "Nespravne zadane parametre, skuste prosim ./stat.py --help"
  else:
    w(sys.argv[2])
else: print "Nespravne zadane parametre, skuste prosim ./stat.py --help"
