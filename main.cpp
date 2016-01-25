#include <stdio.h><A><adsa
#include <cstring>
#include <map>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <list>
#include <cctype>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <regex>
#include <locale>
#include <ctype.h>


using namespace std;
/////////////////////////////////
bool readFile(string filename);
void loops ();
bool spaceCheck(string x);
bool menmonicCheck(string testString);
void parse();
void parser2();
void stepIncrement();
bool validate_Label();
bool validate_Label2();
bool validate_operand(string testString);
void tab() ;
/////////////////////////////////////
void init ();
void init2 ();
int pass1 ();
string getstr (int x);
int getAddressofStartDirective ();
int getMnenmonicLenght (int i);
int getDirectiveLength (int i);
bool isHexa (string s) ;
int getDecimalFromHexa (string s);
void handleFormat2 (int i);
void handleFormat3 (int i);
int stringtoint (string s);
int  isDecimal (string s) ;
string decimaltohexa (int x);
void errorsRecord ();
string print ();
void writelisa (string ans );
bool handleExpression (string expression);
int  isSymbol (string s) ;
bool isExpression (string expression ) ;
////////////////////////////////

void write();
////////////////////////////////pass2////////////////////////////////////////////////



char isIndexed(string operand);
char isIndirect(string operand);
char isImmediate(string operand);
char isBase(string operand,int displacment);
char isPC (string Operand,int displacment);
char isFour(string mnemonic);
int calculateDisplacment(string operand,int indexLocation);
string GetBinaryStringFromHexString (string sHex);
string GetHexFromBin(string sBinary);
string opCodeGeneration(string mnemonic,string operand,int i);
int operandType(string op);
string parseOperand(string op);
string pass2();
void testflags( int i);
string printPass2();
//////////////////////////////
//////////////////////////////
vector<string> myList;
vector<string> stock;
vector<string> label;
vector<string> mnemonics;
vector<string> operand;
vector<string> comment;
vector<int> step;
vector<string> menmonicsData;
vector<int> syntaxError;
vector<int> labelError;


//////////////////////////////

/////////////////////////////////////////////////
// maps for pass 1
map <string , int> optable ;   //  optable [mnenmonec string] = opcode
map <string , int > formattable ; // formattable [mnemonic string] = format
map <string , int > symtable ;  // symtable [label] =  location counter
map <int , string> errtable ;    // errtable [int] = string says the error type to be printed in the file
map <string , int> regtable ;
/////////////////////////////////////////////////////


/////////////////////////////////////////////////////
int AddressList [1000000]  ;
vector<int> ErrorList [1000000]  ;
int programLength = 0 ;
/////////////////////////////////////////////////////
/////////////////////pass2//////////////////////////////
vector<string>objectCode;
char flags[1000][6]; // ***** n i x b p e
bool baseFlag=false;
bool ldbFlag=false;
string baseOperand;
string ldBaseOperand;
bool baseRelative=false;
int symNotFound[1000] ;
vector <int> mod ;
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//// errors variables
int DuplicateLabel = 0 ;
int InvalidMnemonic =1 ;
int NoStartDirective = 2; ////
int WrongStartPosition = 3 ;
int NoEndDirective = 4 ;
int WrongEndPosition = 5 ;
int Type2FormatError = 6 ;
int IndexedAddressingError = 7 ;
int DirectiveOperandError = 8 ;
int RSUBWarning  = 9 ; ////
int EndLabelError = 10 ;
int OddHexalength = 11 ;
int ExtraCharactersError = 12 ;
int AddressOutofrangeError = 13 ;
int RegisterIndexingError = 14 ;
int IllegalOperandError = 15 ;
int MissingMisplacedError = 16 ;
int UnrecognizedError = 17 ;
int AddressNotrelocatable = 18 ;
int inValidExpression = 19 ;
int ORGLabelError = 20;
int EQULabelError = 21 ;
//////////////////////////////////////////

vector <string> startcomment  , endcomment  ;

int expressionNumericalValue;
bool sucesseful = true ;



int main()
{

     // ATEF
     bool first = true ;
    string tempfilename , filename = "" ;
    cout << "Enter the file Name -> " ;
    cin >> tempfilename ;
    if (first)init ();
    cout << endl << endl;
    cout << "*****************************************************************************" <<endl;
    first = false ;
    for (int i=1 ; i <((signed)tempfilename.length())-1 ;i++){
        filename += tempfilename[i];
    }
    bool opened = readFile(filename);
    cout << "*****************************************************************************"<<endl ;
    if (opened) {
       loops ();
       //return 0 ;
    ///////////////////////////////
   programLength = pass1();

    ////////////////////////////
     errorsRecord () ;
    //////////
    string ans = print ();

    ////////////




    ////////////////////////////////////////////// Put here pass2 ///////////////////////

   if (sucesseful == true) {
       ans += "\n\n\n";
       ans += pass2();

   }

cout << "\n\n\n" << ans;


    /////////////////
    writelisa (ans) ;
    if (sucesseful == true) write ();
    init2 ();












    } else {
      cout << "Unable to open file\n\n" ;
    }
    while (true) ;
    return 0;
}


void write(){

    freopen ("OBJFILE.txt","w",stdout);
    string line;

        ///*** header record strrt  ***///

    line=line+"H";
    string tmp= "      ";
    string name = label.at(0); //the stored program name (if any)
    if(name!=" "){
        string toinsert = tmp.substr(0,name.length());
        string toconc = tmp.substr(name.length(),tmp.length()-name.length());
        toinsert = name;
        name= name+toconc;
                line= line+name;  // program name inserted

    }
        else{
        line = line+tmp;
        }

    string stad =""; //address to be inserted
    string ad = operand.at(0);
    if(ad.length()<6){
        int req = 6-ad.length();
        for(int i=0;i<req;i++){
            stad+="0";
        }
    }
    stad+=ad;

    line = line + stad;    //starting address

    string len = decimaltohexa(programLength);
    string length=""; //to be inserted

    if(len.length()<6){
        int req = 6-len.length();
        for(int i=0;i<req;i++){
            length+="0";
        }
    }
    length= length + len; // program length added.
    line = line + length;

    cout<<line+"\n";
    ///*** header record end  ***///

    ///*** Text RECORD START ***///
    int textLen =0;
    string textLenS;
    int c=1;
    int textStart;
    string textStartS;
    string code;


    while (true){
        if (c>=mnemonics.size() ) break;
          if ( mnemonics.at(c)=="END") break ;
        line ="";
        textLen=0;
        textStart=AddressList[c];
        while(true){
         if (c>=mnemonics.size() ) break;
         if(mnemonics.at(c)=="END") break;
            if (mnemonics.at(c)=="RESW" || mnemonics.at(c)=="RESB" || mnemonics.at(c)=="ORG"){
                //cout<<"\n"
                c++;
                break;
            }
            if (mnemonics.at(c)=="EQU") {
                c++;
                continue ;
            }
            code =objectCode.at(c);
            int clen = 0 ;
            int i = c ;
            string mnemonic = mnemonics[i]  ;
           if ( mnemonics[i][0] == '+' && mnemonics[i].length()>1 ) mnemonic = mnemonics[i].substr(1,mnemonics[i].length()-1);
            if (  optable.find(mnemonic) !=  optable.end()  ){
                clen = getMnenmonicLenght (i);
            }
            else{
             clen = getDirectiveLength (i);
             }
             //clen /= 2;
            if(textLen+clen<=30){
                line = line +code;
                c++;
                textLen+=clen;
            }
            else{
                break;
            }
        }
        string conc ="T";
        textStartS = decimaltohexa(textStart);
        if(textStartS.length()<6){
            int req = 6-textStartS.length();
            for(int i=0;i<req;i++){
                conc+="0";
            }
        }
        conc= conc + textStartS;
        string textlenS="";
        string temp = decimaltohexa(textLen) ;
        //cout << temp << " " << temp.length () << endl;
        if(temp.length()<2){
            string k = "0" ;
            k += temp ;
            textlenS += k;
        }
        else{
            textlenS += temp;
        }
        conc += textlenS;

        conc+= line;

        cout<<conc+"\n";


    }

    ///*** End of Text Record ***///




    ///*** Start of Modification Record ****////


    for(int i=0 ; i<mod.size();i++){

        line="";
        line = line + "M";
        int modAdd=AddressList[mod.at(i)];
        string concat ="";
        string modAddS = decimaltohexa(modAdd);
        if(modAddS.length()<6){
            int req = 6-modAddS.length();
            for(int i=0;i<req;i++){
                concat+="0";
            }
        }
        concat+=modAddS;
        line = line+concat;
        string t =objectCode.at(mod.at(i));
        int diff = 5;
        string diffS = decimaltohexa(diff);
        if(diffS.length()<2){
            diffS="0"+diffS;
        }
        line= line +diffS;

        cout<<line+"\n";


    }



    ///*** start of END Record***///

    line="";
    line= line+"E";

    string st =""; //address to be inserted
    string add = operand.at(0);
    if(add.length()<6){
        int req = 6-add.length();
        for(int i=0;i<req;i++){
            st+="0";
        }
    }
    st+=add;

    line = line +st;

    cout<<line;

     fclose (stdout);

    }




void init2 () {
    startcomment.clear ();
    endcomment.clear ();
   symtable.clear ();
    myList.clear ();
 stock.clear ();
label.clear ();
 mnemonics.clear ();
 operand.clear ();
 comment.clear ();
step.clear ();
 menmonicsData.clear ();
 syntaxError.clear ();
 labelError.clear ();
 for (int i=0 ; i <1000000 ;i++)  ErrorList[i].clear ();

  memset (AddressList,0,1000000);



}
void writelisa (string ans) {
    freopen ("LISAFILE.txt","w",stdout);
    cout << ans << endl;
     fclose (stdout);

}
string toUpper (string s) {
transform(s.begin(), s.end(), s.begin(), ::toupper);
return s ;
  //transform(s.begin(), s.end(), s.begin(), ::toupper);
}
string print () {
    string ans = "" ;
    ans += ">>  Source Program statements with value of LC indicated \n\n" ;
    ans += "      .23456789012345678901234567890123456                              \n" ;
   bool assembleError = false ;
   for (int i=0 ; i <(signed)startcomment.size();i++ ){
    ans += startcomment[i] ;
    ans+='\n';
   }
   for (int i=0;i<(signed)label.size();i++) {
         if ( label [i] == " " && mnemonics[i] == " " && operand [i] == " " && comment [i] == " "  )  {
                ans += '\n' ;
                continue ;
            }
      if (label [i][0] == '.') {
        if ( label[i].length () > 66) {
            for (int j=0; j<66 ; j++)  {
                ans += label [i][j];
            }
            ans+='\n';
            continue ;
        }
        ans+= label [i] ;
        ans+= '\n';
        continue ;
    }
    string address = toUpper(decimaltohexa (AddressList[i])) ;
    for (int j=0 ; j < 6-((signed)address.length()) ;j++) {
        ans+='0' ;
    }
    ans += address ;
    //cout << ans << endl;
    ans += ' ' ;
    ans += label [i] ;

    for (int j=0 ; j < 9-((signed)label[i].length()) ;j++) {
        ans+=' ' ;
    }
    ans += mnemonics[i] ;
    for (int j=0 ; j < 8-((signed)mnemonics[i].length()) ;j++) {
        ans+=' ' ;
    }
    ans+=operand [i];
    for (int j=0 ; j < 18-((signed)operand[i].length()) ;j++) {
        ans+=' ' ;
    }
    for (int j=0 ; j < 31 ;j++) {
        if (j>= (signed)comment[i].length()) break ;
        ans+= comment[i][j] ;
    }
    ans += '\n' ;
    for (int j=0;j<(signed)ErrorList[i].size();j++) {
         ans += " " ;
         ans+=  errtable [ErrorList[i][j]] ;
         ans += '\n' ;
         // RSUBWarning NoStartDirective
         if (  ! (ErrorList[i][j] == RSUBWarning || ErrorList[i][j] == NoStartDirective || ErrorList[i][j]==EndLabelError)      ){
            assembleError = true ;
         }
     }
   }
    for (int i=0 ; i <(signed)endcomment.size();i++ ){
    ans += endcomment[i] ;
    ans+='\n';
   }
   ans += '\n' ;
    ans += "PROGRAM LENGTH (in Hexa)  : " ;
    ans += toUpper(decimaltohexa (programLength)) ;
    ans += '\n' ;
    ans += '\n';
    if (assembleError == false) {
    ans += ">>    e n d    o f   p a s s   1\n\n" ;
    ans += ">>   *****************************************************\n" ;
    ans += ">>    s y m b o l     t a b l e   (values in decimal)\n\n" ;
    ans += "        name         value     Absol/Reloc\n" ;
    ans += "        ----------------------------------\n" ;
    for (std::map<string,int>::iterator it=symtable.begin(); it!=symtable.end(); ++it) {
        ans += "        " ;
        string first = it->first;
        transform(first.begin(), first.end(), first.begin(), ::tolower);
        ans += first ;
        string temp = it->first ;
        for (int j=0 ; j<8-((signed)temp.length ()) ; j++) {
            ans+=' ' ;
        }
        ans+="     ";
        string address = getstr (it->second);
        for (int j=0 ; j<5-((signed)address.length ()) ; j++) {
            ans+=' ' ;
        }
        ans += address ;
        ans += "    Relocatable\n";
    }
    ans += '\n' ;
    ans += ">>   *****************************************************\n" ;
    } else {
        sucesseful = false;
    ans += ">>    i n c o m p l e t e    a s s e m b l y\n" ;
    }
    /* cout << decimaltohexa (AddressList[i])<<" " ;

     << label [i]<< "          " << mnemonics[i] << " " << operand[i] << " " <<comment[i]<< endl;
     for (int j=0;j<(signed)ErrorList[i].size();j++) {
         cout << errtable [ErrorList[i][j]] << endl;
     }
    } */
    return ans ;

}
void errorsRecord () {
for (int i=0 ; i < (signed) syntaxError.size () ; i++) {
        ErrorList[syntaxError[i]].push_back(MissingMisplacedError);
    }
    for (int i=0 ; i < (signed) labelError.size () ; i++) {
        ErrorList[labelError[i]].push_back(UnrecognizedError);
    }
}
void tab()
{
    for(int unsigned i =0 ; i<myList.size(); i++)
    {
        string temp = myList.at(i);
        for(int unsigned j =0; j<temp.size(); j++)
        {
            if(temp.at(j) == '\t')
            {   temp = regex_replace(temp,regex("\\t"),string(""));
                myList.at(i) = temp ;
                syntaxError.push_back(i);
            }
        }
    }
}

void loops () {
    tab();
    int shift = 0  ;
    for(int unsigned i=0;i<myList.size();i++) {
        if ( myList [i][0] == '.') shift++;
        else break ;
    }

for(int unsigned i=0;i<myList.size();i++){
        if (myList.size()>=1) {
            if ( myList [i][0] == '.' &&  myList [i].size () > 66  ) continue ;
        }
        bool x=spaceCheck(myList.at(i));
        if(x==false){
           //cout<< "syntax error"<<'\n';
          syntaxError.push_back(i-shift);
        }
    }
    parser2();

//    for(int unsigned i=0;i<menmonics.size();i++)
//    {
//        if(i>menmonics.size()){
//        break;
//        }
//        if(menmonicCheck(menmonics.at(i))==false){
//            cout << "menmonic error"<<'\n';
//            cout<<menmonics.at(i)<<'\n' ;
//            return 0;
//        }
//
//    }

//    for(int unsigned i=0 ; i<operand.size(); i++){
//        if(validate_operand(operand.at(i))==false){
//            cout << "operand error"<<'\n';
//            cout<<menmonics.at(i)+" "<<operand.at(i)<<i<<endl ;
//            return 0;
//        }
//
//    }

         validate_Label2();

    //stepIncrement();
   /* for(int unsigned i=0;i<mnemonics.size();i++)
    {
        cout<<label.at(i);
        cout<<" " ;
        cout<<mnemonics.at(i);
        cout<<" " ;
        cout<<operand.at(i);
        cout<<" " ;
        cout<<comment.at(i);
        cout<<" " ;
       // cout<<step.at(i);
        cout<<" "<<endl;

    }*/
//


}


bool readFile(string filename){

          string line = "" ;
          ifstream myfile (filename + ".txt");
          if (myfile.is_open())
          {
            while ( getline (myfile,line) )
            {
               // if (line.length() == 0 ) continue ;

         for(unsigned int l = 0; l < line.length(); l++)
         {
                line[l] = toupper(line[l]);
         }
              myList.push_back(line);
              cout << line << '\n';
            }
            myfile.close();
            return true ;
          }

          else{


                  return false;
          }
          if (myList.size() == 0) return false ;
//        string line2;
//
//        ifstream myfile2 ("menmonics.txt");
//        if (myfile2.is_open())
//        {
//          while ( getline (myfile2,line2) )
//          {
//            menmonicsData.push_back(line2);
//
//          }
//          myfile2.close();
//        }
//
//        else{
//
//                cout << "Unable to open file";
//        }

}
//***********************************************************space checker***********************************
bool spaceCheck(string testString){
    regex e("(........ ......  .{0,48})|(........ ......)|(\\.(.*))|(.*)");

    //regex e("\\s*(([A-Z](\\w*|\\d*))|\\s{8})\\s([A-Z]+)\\s{2}(\\d{1,18}|(\\d{1,16},X)|(@\\d{1,17})|(#\\d{1,17})|())");

    int length=testString.length();

    bool match=regex_match(testString,e);
    if(length>66){
        match=false;
    }

        return match;

}

//*************************************************************check menmonic *****************************************


//bool menmonicCheck(string testString){
//
//       // return menmonics;
//      //check for presence of menmonic in database
//      for(int unsigned i=0;i<menmonicsData.size();i++){
//          //  cout<<menmonicsData.size();
//        if(testString.compare(menmonicsData.at(i))==0 || testString.compare(" ")==0){
//           return true ;
//        }
//
//        }
//        return false;
//      }



//************************************************************step increment*******************************************
void stepIncrement(){
    string format2[] ={"ADDR","CLEAR","COMPR","DIVR","MULR","RMO","SHIFTL","SHIFTR","SUBR","TIXR"};
    for(unsigned i=0;i<mnemonics.size();i++)

    {
         bool flag=false;
        //comments
        string space=" ";
        if(space.compare(mnemonics.at(i))==0){
            step.push_back(0);
            flag=true;
        }
        if(flag==true){
                continue;
        }

        //reserve byte

        string resb="RESB";
        if(resb.compare(mnemonics.at(i))==0){
        string n=operand.at(i);
        std::istringstream ss2(n);
        int conv;
        ss2>>conv;
        cout<<std::hex ;

        step.push_back(conv);
        flag=true;
        }
        if(flag==true){
            continue;
        }

        // reserve word
        string resw="RESW";
        if(resw.compare(mnemonics.at(i))==0){

        string n=operand.at(i);
        std::istringstream ss2(n);
        int conv;
        ss2>>conv;
        conv=3*conv;
      //  cout<<std::hex <<46;

        step.push_back(conv);
        flag=true;
        }
        if(flag==true){
            continue;
        }


        // BYTE directive
        string byte="BYTE";
        bool coma=false;
        bool out =false;
        int counter=0;
        if(byte.compare(mnemonics.at(i))==0){
            string myByte=operand.at(i);
            for(int i=0;i<(signed)myByte.size();i++){
                if(myByte.at(i)=='\''){
                    coma=true;
                }

                if(coma==true ){
                    if(myByte.at(i)!='\''){
                        counter++;
                        out=true;
                    }
                    else if (out==true){
                       break;
                    }
                }
            }
            if(myByte.at(0)=='C'){
                ostringstream  n ;
                 n<<counter;
               //  n.str();
               // string n=counter;
                std::istringstream ss2(n.str());
                int conv;
                ss2>>conv;
               cout<<std::hex ;
                step.push_back(conv);
                flag=true;
            }
            else if(myByte.at(0)=='X'){
                counter=counter/2;
                ostringstream  n ;
                 n<<counter;
               //  n.str();
               // string n=counter;
                std::istringstream ss2(n.str());
                int conv;
                ss2>>conv;
                cout<<std::hex ;
                step.push_back(conv);
                flag=true;
            }


        }
        if(flag==true){
            continue;
        }


        //test 1 (format 2)
        for(unsigned j=0;j<10;j++){
                flag=false;
                string temp1=format2[j];
            if(temp1.compare(mnemonics.at(i))==0){
                step.push_back(2);
                flag=true;
                break;
            }

        }
        if(flag==true){
            continue;
        }

        //test 2 (format 4);

         string str=operand.at(i);
         char myArray[str.size()+1];
        strcpy(myArray, str.c_str());
         if(myArray[0]=='+'){
            step.push_back(4);
            continue;
         }

        else{
            step.push_back(3);
        }
    }

}

  //********************************validate operand******************************************************************************
  bool validate_operand(string testString){

          regex e("\\d{1,18}|(\\d{1,16},X)|(@\\d{1,17})|(#\\d{1,17})|([A-Z](\\w{0,17}|\\d{0,17}))|([A-Z](\\w{0,16}|\\d{0,15}),X)|(@[A-Z](\\w{0,16}|\\d{0,16}))|(#[A-Z](\\w{0,16}|\\d{0,16}))|(\\*)|(C'.{1,15}')|(X'(\\d|[A-F]){1,14}')|(\\s)|((A|X|L|B|S|T|F),(A|X|L|B|S|T|F))|([A-Z](\\w*|\\d*)-\\d+)|([A-Z](\\w*|\\d*)\\+\\d+)|(\\d+\\+([A-Z](\\w*|\\d*)))");
       bool match=regex_match(testString,e);
       //cout<< testString<<'\n' ;
       return match ;
  }
  //*************************************parser2********************************************************************************
  void parser2(){

      for(int i=0 ; i<(signed)myList.size(); i ++){
        string t =myList.at(i);
        if (t.size() == 0 ) {
            label.push_back(" ");
           operand.push_back(" ");
           comment.push_back(" ");
           mnemonics.push_back(" ");
           continue ;
        }
        if(t.at(0) == '.'){
           label.push_back(t);
           operand.push_back(" ");
           comment.push_back(" ");
           mnemonics.push_back(" ");

       }
       else{
           string l = t.substr(0,8);
           l = regex_replace(l,regex("\\s"),string(""));
           if(l.size() == 0){
               l = " ";
           }
           label.push_back(l);


           if(t.size()<=14){
           string m = t.substr(9,t.size()-9);
           m = regex_replace(m,regex("\\s"),string(""));
           if(m.size() == 0){
               m = " ";
           }
           mnemonics.push_back(m);
           operand.push_back(" ");
           comment.push_back(" ");
           }

           if(t.size() >14){
           string m = t.substr(9,6);
           m = regex_replace(m,regex("\\s"),string(""));
           if(m.size() == 0){
               m = " ";
           }
           mnemonics.push_back(m);

               int des = 0 ;
                   if(t.size()-17 >18){
                       des = t.size()-17-(t.size()-34);
                   }
                   else{
                       des = t.size()-17 ;
                   }

               string op = t.substr(17,des);
                int t = op.find_last_of("'");
                if(t == -1){
                    op = regex_replace(op,regex("\\s"),string(""));
                }
                else{
                    op.resize(t+1);
                }
               operand.push_back(op);
           }


           if(t.size() > 34){
               string com = t.substr(35,t.size()-34);
               comment.push_back(com);
           }
           else{
               comment.push_back(" ");
           }
         }
        }
    }
  bool validate_Label2(){
      for(int unsigned i=0 ; i <label.size() ; i++){
        regex e("([A-Z](\\w|\\d){0,7})|((\\.)(.*))|\\s");
        bool match=regex_match(label.at(i),e);
        if(match == false){
           // cout<<label.at(i)<<" " << i <<'\n' ;
           labelError.push_back(i);
            return false ;
        }
      }
      return true ;
  }













  ///////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////


 void init () {
    // optable , formattable initialization
    freopen("mnemonic.txt","r",stdin);
    std::stringstream ss;
    string mnemonic , hexastring;
     int format , decimaltemp ;
    for (int i = 0 ; i < 39 ; i++) {
    cin >> mnemonic >> format >> hexastring ;
    formattable [mnemonic] = format ;
    ss << std::hex << hexastring ;
    ss>>decimaltemp ;
    optable [mnemonic] =  decimaltemp ;
    ss.clear();
    }
    fclose (stdin);

    // errortable initialization // new ERRORS may be added
    errtable [DuplicateLabel] = "*** ERROR: Duplicate label." ;
    errtable [InvalidMnemonic] = "*** ERROR: Invalid mnemonic." ;
    errtable [NoStartDirective] = "*** Warning: No START directive." ;
    errtable [WrongStartPosition] = "*** ERROR: Wrong START directive position." ;
    errtable [NoEndDirective] = "*** ERROR: No END Directive." ;
    errtable [WrongEndPosition] = "*** ERROR: Wrong END Directive Position." ;
    errtable [Type2FormatError] = "*** ERROR: Type 2 Format Error." ;
    errtable [IndexedAddressingError] = "*** ERROR:  Indexed Addressing Error." ;
    errtable [DirectiveOperandError] =  "*** ERROR: Directive operand Error." ;
    errtable [RSUBWarning] = "*** Warning: RUSB operand ignored." ;
    errtable [EndLabelError] = "*** Warning : END Directive with Label.";
    errtable[OddHexalength] = "*** ERROR: ODD length of a Hexa Decimal constant.";
    errtable [AddressOutofrangeError] = "*** ERROR: Address Outoff Range Error. " ;
    errtable [ExtraCharactersError] = "*** ERROR: Extra Characters at the End of the Statment Error. " ;
    errtable [RegisterIndexingError] = "*** ERROR: Register Indexing Error." ;
    errtable[IllegalOperandError] = "*** ERROR : Illegal Operand Error." ;
    errtable [MissingMisplacedError] = "*** ERROR : Missing or Misplaced Operand Error.";
    errtable [UnrecognizedError] = "*** ERROR : Unrecognized operation code Error. ";
    errtable [AddressNotrelocatable] = "*** ERROR : Address Not relocatable Error.";
     errtable [inValidExpression] = "*** ERROR : Invalid Expression Error. ";
     errtable [ORGLabelError] = "*** ERROR : Invalid ORG Label field Error. ";
     errtable [EQULabelError] = "*** ERROR : Invalid EQU Label field Error. ";
    ////////////////////////
    memset (AddressList,0,1000000);
   // memset (ErrorList,-1,1000000);
    ////////////////////
    regtable ["A"] = 0 ;
    regtable ["X"] = 1 ;
    regtable ["L"] = 2 ;
    regtable ["B"] = 3 ;
    regtable ["S"] = 4 ;
    regtable ["T"] = 5 ;
    regtable ["F"] = 6 ;
    regtable ["PC"] = 8 ;
    regtable ["SW"] = 9 ;

}
bool isExpression (string expression ) {
   string op1="" , op2="" , operate="" ;
    int operatorIndex = -1;
  for (int i=0 ; i<expression.size() ; i++) {
    if (expression [i] == '+' || expression [i] == '-' || expression [i] == '*' || expression [i] == '/') {
        operatorIndex = i ;
        operate += expression [i] ;
        break ;
    }
    if (  ! ((expression [i] >= '0' && expression [i] <='9' ) || (expression [i] >= 'A' && expression [i] <='Z' ))   ) {
        return false;
    }
 }
  if (operatorIndex != -1) {
            if (operatorIndex == 0 || operatorIndex == expression.size() - 1) {
         return false ;
             }
             for (int i=0 ; i<operatorIndex;i++) op1 += expression [i] ;
for (int i=operatorIndex+1 ; i<expression.size();i++) op2 += expression [i] ;

   int isDec1 = isDecimal(op1);
   int isSym1 = isSymbol(op1);
   int isDec2 = isDecimal(op2);
   int isSym2 = isSymbol(op2);
   int value1 , value2 ;
   if (  isDec1 +  isSym1 == 0    ) return false ;
   if (  isDec2 +  isSym2 == 0    ) return false ;
  } else {
     int isDec = isDecimal (expression);
   int isStr = isSymbol (expression);
   // cout << expression <<(isDec + isStr) << endl;
   if (isDec + isStr == 0) return false ;
  }
  return true ;
}
bool handleExpression (string expression) {
    if (expression == " " ) return false;
   string op1="" , op2="" , operate="" ;
bool symbolic1=false , symbolic2=false  , validExp=false;
 expressionNumericalValue = 0 ;
 int operatorIndex = -1;
 for (int i=0 ; i<expression.size() ; i++) {
    if (expression [i] == '+' || expression [i] == '-' || expression [i] == '*' || expression [i] == '/') {
        operatorIndex = i ;
        operate += expression [i] ;
        break ;
    }
    if (  ! ((expression [i] >= '0' && expression [i] <='9' ) || (expression [i] >= 'A' && expression [i] <='Z' ))   ) {
        return false;
    }
 }
 if (operatorIndex != -1) {
if (operatorIndex == 0 || operatorIndex == expression.size() - 1) {
 return false ;
}
for (int i=0 ; i<operatorIndex;i++) op1 += expression [i] ;
for (int i=operatorIndex+1 ; i<expression.size();i++) op2 += expression [i] ;


   int isDec1 = isDecimal(op1);
   int isSym1 = isSymbol(op1);
   int isDec2 = isDecimal(op2);
   int isSym2 = isSymbol(op2);
   int value1 , value2 ;
   if (  isDec1 +  isSym1 == 0    ) return false ;
   if (  isDec2 +  isSym2 == 0    ) return false ;


   if ( expression [operatorIndex]  == '*' || expression [operatorIndex]  ==  '/'  ) {
      if (isDec1 == 1 && isDec2 == 1) {
            value1 = stringtoint (op1);  value2 = stringtoint (op2);
        if (expression [operatorIndex] == '*') expressionNumericalValue = value1 * value2 ;
         else {
        if (value2 == 0) return false ;
        expressionNumericalValue = value1 / value2 ;
         }
      } else return false;
      return true ;
   }
    if ( expression [operatorIndex]  == '+'  ) {
      if ( ! (isSym1 == 1 && isSym2 == 1)) {


                       if (isDec1==1) value1 = stringtoint (op1);
                       else {
                      if ( symtable.find(op1) == symtable.end() )return false ;
                       else value1 = symtable [op1];
                       }


                        if (isDec2==1) value2 = stringtoint (op2);
                         else {
                          if ( symtable.find(op2) == symtable.end() ) return false ;
                           else value2 = symtable [op2];
                         }
        expressionNumericalValue = value1 + value2 ;
      return true ;
    } else return false;
    }

    if (expression [operatorIndex]  == '-' ) {
                      if (isDec1==1) value1 = stringtoint (op1);
                       else {
                      if ( symtable.find(op1) == symtable.end() )return false ;
                       else value1 = symtable [op1];
                       }


                        if (isDec2==1) value2 = stringtoint (op2);
                         else {
                          if ( symtable.find(op2) == symtable.end() ) return false ;
                           else value2 = symtable [op2];
                         }
        expressionNumericalValue = value1 - value2 ;
      return true ;

    }




} else {
   int isDec = isDecimal (expression);
   int isStr = isSymbol (expression);
   // cout << expression <<(isDec + isStr) << endl;
   if (isDec + isStr == 0) return false ;
   int value ;
   if (isDec == 1) value = stringtoint(expression);
   else {
    if ( symtable.find(expression) == symtable.end() ) return false ;
    else value = symtable [expression];
   }
  expressionNumericalValue = value ;
  return true ;
}
}


int pass1 () {
    int LOCCTR  , t = 1 ;
    if ((signed) label.size() >= 1) {
    while (label[0][0] == '.') {
        startcomment.push_back ( label [0]) ;
        label.erase (label.begin());
        mnemonics.erase (mnemonics.begin());
        operand.erase (operand.begin());
        comment.erase (comment.begin());
       if ((signed) label.size() == 0) break;
    }
    while ( label [0] == " " && mnemonics[0] == " " && operand [0] == " " && comment [0] == " "   ) {
        string t = "" ;
        t+= '\n';
        startcomment.push_back (t) ;
        label.erase (label.begin());
        mnemonics.erase (mnemonics.begin());
        operand.erase (operand.begin());
        comment.erase (comment.begin());
       if ((signed) label.size() == 0) break;
    }
    }
    if ((signed) label.size() >= 1) {
    while (label[(signed)label.size()-1][0] == '.') {
        endcomment.push_back ( label [(signed)label.size()-1]) ;
        label.erase (label.begin()+(((signed)label.size())-1));
        mnemonics.erase (mnemonics.begin()+(((signed)mnemonics.size())-1));
        operand.erase (operand.begin()+(((signed)operand.size())-1));
        comment.erase (comment.begin()+(((signed)comment.size())-1));
        if ((signed) label.size() == 0) break;
    }
    while ( label [(signed)label.size()-1] == " " && mnemonics[(signed)label.size()-1] == " " && operand [(signed)label.size()-1] == " " && comment [(signed)label.size()-1] == " "   ) {
        string t = "" ;
        t+= '\n';
        startcomment.push_back (t) ;
        label.erase (label.begin()+(((signed)label.size())-1));
        mnemonics.erase (mnemonics.begin()+(((signed)mnemonics.size())-1));
        operand.erase (operand.begin()+(((signed)operand.size())-1));
        comment.erase (comment.begin()+(((signed)comment.size())-1));
       if ((signed) label.size() == 0) break;
    }
    }
    if (  mnemonics [0] == "START"    ) {
        LOCCTR = getAddressofStartDirective ();
        if (LOCCTR >= 12345) {
                // AddressOutofrangeError
                 ErrorList [0].push_back(AddressOutofrangeError) ;
             }
    }
    else{
        LOCCTR = 0 ;
        ErrorList [0].push_back(NoStartDirective) ;
        t=0;
    }
    AddressList [0]  = LOCCTR ;
    for (int i = t; i <(signed)label.size() ;i++) {
            if ( label [i] == " " && mnemonics[i] == " " && operand [i] == " " && comment [i] == " "  )  {
                continue ;
            }
       if ( label[i] [0] != '.'  ) {
            if (  label [i] != " "   ) {
                if ( symtable.find(label[i]) == symtable.end() ) symtable [label[i]] = LOCCTR ;
                else ErrorList [i].push_back (DuplicateLabel);
            }
            if (  mnemonics[i] == "EQU" || mnemonics [i] == "ORG"   )  {
                if (mnemonics [i] == "ORG" && label[i] != " ") {
                    ErrorList [i].push_back (ORGLabelError);
                    continue ;
                }
                if (  mnemonics [i] == "EQU" && label[i] == " "  ) {
                    ErrorList [i].push_back (EQULabelError);
                    continue ;
                }
                bool validExp = handleExpression (operand[i]);
                if (validExp == false) {
                    ErrorList [i].push_back(inValidExpression) ;
                } else {
                if (mnemonics[i] == "EQU") {
                    symtable [label[i]] = expressionNumericalValue;
                    AddressList[i] = LOCCTR ;
                } else {
                  AddressList[i] = expressionNumericalValue;
                  LOCCTR = expressionNumericalValue;
                }
                }
            }
            else {
            int offset = 0 ;
            string mnemonic = mnemonics[i]  ;
           if ( mnemonics[i][0] == '+' && mnemonics[i].length()>1 ) mnemonic = mnemonics[i].substr(1,mnemonics[i].length()-1);
            if (  optable.find(mnemonic) !=  optable.end()  ){
                offset = getMnenmonicLenght (i);
            }
            else{
             offset = getDirectiveLength (i);
             }
            if (offset == -1) {
                ErrorList [i].push_back(InvalidMnemonic) ;
                offset = 0 ;
            }
             AddressList[i] = LOCCTR ;
             LOCCTR += offset ;
             if (LOCCTR >= 12345  ) {
                if ( ! ( ( i+1 == (signed) label.size()) &&  mnemonics[i] == "END" )  )  {
                     // AddressOutofrangeError
                 ErrorList [i].push_back(AddressOutofrangeError) ;
                }

             }
            }
       }


    }
   // AddressList[(signed)label.size( )-1] = LOCCTR ;
    // return the program length
    if (mnemonics[label.size()-1] != "END") ErrorList [label.size()-1].push_back(NoEndDirective);
    return AddressList [label.size()-1] - AddressList [0] ;
}


int getAddressofStartDirective () {
    if (operand[0] == " ") {
        ErrorList [0].push_back (MissingMisplacedError);
        return 0 ;
    }
    if ( (operand [0] [0] >= 'A' && operand [0] [0] <= 'F') ) {
        ErrorList [0].push_back (DirectiveOperandError);
        return 0 ;
        }
    if (  (operand [0].length() > 5 || !isHexa (operand[0])) || (  operand [0].length() == 5 &&  operand [0][0] != '0'   )) {
        string subhex = "0" ;
        for (int j=0 ; j<4 ;j++ ) {
            if ( (operand [0] [j] >= 'A' && operand [0] [j] <= 'F')   ) {
                subhex+= operand [0] [j] ;
            } else if (  (operand [0] [j] >= '0' && operand [0] [j] <= '9')   ) {
               subhex += operand [0] [j] ;
            } else {
               break ;
            }
        }
        ErrorList [0].push_back (ExtraCharactersError);
       return getDecimalFromHexa (subhex) ;
    }
    return getDecimalFromHexa (operand[0]) ;

}

bool isHexa (string s) {
    for (int i = 0 ; i <(signed)s.length() ; i++) {
        if (!((s[i] >= '0' && s[i] <= '9' ) || (s[i] >= 'A' && s[i] <= 'F'))) return false ;
    }
    return true ;
}

int getDecimalFromHexa (string s) {
    std:: stringstream ss;
    ss << std::hex << s ;
    int decimaltemp ;
     ss>>decimaltemp ;
     return decimaltemp ;
}

int getMnenmonicLenght (int i ) {
    if ( mnemonics[i] == "RSUB" ||   mnemonics[i] == "+RSUB" ) {
        if ( operand[i] != " "  ) ErrorList [i].push_back(RSUBWarning);
        return mnemonics[i] == "RSUB" ? 3 : 4 ;
    }
    string mnemonic = mnemonics[i]  ;
    int formatoffset = 0 ;
    if ( mnemonics[i][0] == '+' && mnemonics[i].length()>1) {
        mnemonic = mnemonics[i].substr(1,mnemonics[i].length()-1);
        formatoffset = 1 ;
    }
    if (  optable.find(mnemonic) == optable.end()    ) {
        ErrorList [i].push_back(InvalidMnemonic);
        return 0 ;
    }
    int format = formattable [mnemonic] ;
    if (format == 2) handleFormat2 (i);
    else if (format == 3) handleFormat3 (i);
    return format + formatoffset ;
}

void handleFormat2 (int i ) {
   if ( operand [i].length() == 1   ) {
       if ( !((mnemonics [i] == "TIXR" || mnemonics[i] == "CLEAR") && (formattable.find(operand[i]) != formattable.end()))  ) {
           ErrorList [i].push_back(Type2FormatError);
       }
       return ;
   }
   if (operand[i][1] != ',') {
       ErrorList [i].push_back(Type2FormatError);
        return ;
   }
   if ( operand [i].length() == 3  ) {
       string r1 = "" , r2 = "" ;
       r1 += operand [i][0]; r2 += operand [i][2];
       if (regtable.find(r1) == regtable.end()) {
           ErrorList [i].push_back(Type2FormatError);
           return ;
       }
       if ( mnemonics [i] == "SHIFTR"  || mnemonics [i] == "SHIFTL" ) {
            if ( isDecimal (operand[i]) == 0   ) {
                ErrorList [i].push_back(Type2FormatError);
                return ;
            }
       } else {
           if (regtable.find(r2) == regtable.end()) {
               ErrorList [i].push_back(Type2FormatError);
              return ;
            }
         return ;
       }
   }
   ErrorList [i].push_back(Type2FormatError);
   return ;
}
void handleFormat3 (int i ) {
    int length = operand[i].length() ;
    if (operand [i] == "*") return ;
    bool   special = false , indexing = false , ni = false ;
    int numbers = 0 , ch = 0 ;
    for (int j=0 ; j<length ; j++) {
        if (operand [i][j] >= 'A' && operand [i][j] <= 'Z' ) ch++;
        else if (  operand [i][j] >= '0' && operand [i][j] <= '9'   ) numbers++;
        else  if (  (operand [i][j] == '#' || operand [i][j] == '@')  && (  j==0 ) && length > 1) ni = true ;
        else if (  operand [i][j] == ','  && j== length-2 && length > 2) indexing = true;
        else special = true ;
    }
    if (ch == length) return ;
    if (numbers == length) {
        if (numbers<=4) return ;
        else {
           ErrorList [i].push_back (ExtraCharactersError);
           return ;
        }
    }
    if (special == true) {
      if ( (operand [i][0] == '#' || operand [i][0] == '@')     ) {
         if (length == 1) {
        ErrorList [i].push_back (MissingMisplacedError);
        return ;
         }
      }
if ( isExpression ( operand [i] ) ) return ;
      ErrorList [i].push_back (IllegalOperandError);
      return ;
    }
    if (  indexing == false && ni == false ) {
        if (operand [i][0] >= '0' && operand [i][0] <= '9' ) {
           ErrorList [i].push_back (ExtraCharactersError);
           return ;
        }
    }
    if (indexing == true) {
        if (  operand [i][length-1] != 'X'      ) {
           ErrorList [i].push_back (RegisterIndexingError);
           return ;
        }
        if (  ni == true   ) {
           ErrorList [i].push_back (IndexedAddressingError);
           return ;
        }
        if (ch + 1 == length && ch != 0) return  ;
        if (numbers + 2 == length && numbers  != 0) {
           if (numbers > 4) {
            ErrorList [i].push_back (ExtraCharactersError);
           return ;
           }
           return ;
        }
          if (operand [i][0] >= '0' && operand [i][0] <= '9' ) {
           ErrorList [i].push_back (ExtraCharactersError);
           return ;
        }
        return ;
    }
    if (  ni == true   ) {
        if (ch+1 == length && ch != 0 ) return ;
        if (numbers+1 == length && numbers != 0) {
          if (numbers > 4) {
            ErrorList [i].push_back (ExtraCharactersError);
           return ;
           }
           return ;
        }
        if (operand [i][1] >= '0' && operand [i][1] <= '9' ) {
           ErrorList [i].push_back (ExtraCharactersError);
           return ;
        }
        return ;
    }











   /*  if (length <= 3) {
     // RegisterIndexingError
     if ( length >= 2  ) {
         if ( (operand[i][length-1]  != 'X' && operand [i][length-2] == ',' ) ){
             ErrorList [i].push_back (RegisterIndexingError);
             return ;
         }
     }


     }
     if (length >3) {
         if ( (operand[i][length-1]  == 'X' && operand [i][length-2] == ',' ) && (operand [i][0] == '#' || operand [i][0] == '@') ){
             ErrorList [i].push_back (IndexedAddressingError);
             return ;
         }
         if (  operand [i][0] == '#'   || operand [i][0] == '@'     ) {
            string sub = "" ;
            for (int j=1 ; j<operand[i].length() ; j++ ) sub+= operand [i][j] ;
            if (  isDecimal(sub) != 0   ) {
                if (  sub.length() > 4 ) {
                   ErrorList [i].push_back (ExtraCharactersError);
                   return ;
                }
                int decimalAddress = stringtoint(sub) ;
                if (  ! (decimalAddress>=-2048 && decimalAddress<=2047)    ) {
                    ErrorList [i].push_back (AddressOutofrangeError);
                   return ;
                }
            }
         }
     }*/
}

int  getDirectiveLength (int i ) {
    if (mnemonics [i] == "START") {
        ErrorList [i].push_back (WrongStartPosition);
        return 0 ;
    } else if (  mnemonics [i] == "END" ) {
        if (  label [i] != " "   ) {
            ErrorList [i].push_back (EndLabelError);
            return 0 ;
        }
        if (i+1!=(signed)label.size()) {
            ErrorList [i].push_back (WrongEndPosition);
            return 0 ;
        }
        if (  operand[i] != " " ) {
                if ( isDecimal( operand[i] ) != 0  ) {
                   ErrorList [i].push_back (AddressNotrelocatable);
                   return 0 ;
                }
            if ( symtable.find(operand[i]) == symtable.end() ){
                ErrorList [i].push_back (DirectiveOperandError);
                   return 0 ;
            }
               /* else ErrorList [i].push_back (DuplicateLabel);



            if (  operand[i].length () > 2 ) {
                if
            }
                if ()
                if (  mnemonics [0] == "START"  ) {
                   if (  !(operand [i] == label [1] || operand [i] == getstr(AddressList[1]))  ){
                ErrorList [i].push_back (DirectiveOperandError);
               return 0 ;
                }
                } else {
                    if (  !(operand [i] == label [0] || operand [i] == getstr(AddressList[0]))  ){
                ErrorList [i].push_back (DirectiveOperandError);
               return 0 ;
                }
                }*/

            return 0 ;
        }
        return 0 ;
    } else if (  mnemonics [i] == "BYTE"  ) {
        ////////////////////
        if (operand [i].length() <= 3) {
            ErrorList [i].push_back (DirectiveOperandError);
            return 0 ;
        }
        int temp =  0;
        if ( operand [i][0] == 'C' && operand [i][1] == '\'' && operand[i][operand[i].length()-1] == '\''  ) {
            for (int j = 2 ; j < (signed)operand [i].length ()-1 ; j++ ) temp++;
            if (temp>18) {
                ErrorList [i].push_back (DirectiveOperandError);
                return 0 ;
            }
            return temp ;
        } else if (operand [i][0] == 'X' && operand [i][1] == '\'' && operand[i][operand[i].length()-1] == '\'' ) {
             string s = "" ;
             for (int j=2 ; j<(signed)operand[i].length()-1;j++) {
                 s+= operand [i][j] ; temp++;
             }
             if (!isHexa (s)  || temp > 17) {
                 ErrorList [i].push_back (DirectiveOperandError);
                  return 0 ;
             }
             if (temp%2 == 1) {
               ErrorList [i].push_back (OddHexalength);
                  return 0 ;
             }
             return temp/2 ;
        } else {
            ErrorList [i].push_back (DirectiveOperandError);
            return 0 ;
        }
    } else if ( mnemonics [i] == "RESW" || mnemonics [i] == "RESB"   ) {
        int factor = 1 ;
        if (mnemonics [i] == "RESW") factor = 3 ;
        if (isDecimal (operand[i]) != 1  || operand[i].length() > 4) {
            ErrorList [i].push_back (DirectiveOperandError);
            return 0 ;
        }
        return stringtoint (operand[i]) * factor ;
    } else if (  mnemonics [i] == "WORD") {
        if ( isExpression ( operand [i] ) ) return 3 ;
        string temp = "" ;
        vector <string> commas ;
        for (int j = 0 ; j < (signed)operand[i].size();j++) {
          if ( operand [i][j] != ','  ) {
            temp+= operand [i][j] ;
            if (j+1 == (signed)operand[i].size()) commas.push_back(temp);
          }
          else {
            commas.push_back(temp);
            temp="" ;
          }
        }
        if ((signed)commas.size() == 1) {
            if ( isDecimal(operand [i]) == 0) {
               ErrorList [i].push_back (DirectiveOperandError);
               return 0 ;
            } else return 3 ;
        } else {
            for (int j = 0 ; j < (signed)commas.size();j++) {
                if ( isDecimal(commas[j]) == 0)  {
                   ErrorList [i].push_back (DirectiveOperandError);
                   return 0 ;
                }
            }
            return 3*(signed)commas.size();
        }
    }
    ErrorList [i].push_back (InvalidMnemonic);
    return 0 ;
}

int isDecimal (string s  ) {
    int t = (s[0] == '-') ? 1 : 0 ;
    for (int i = t ; i < (signed)s.length() ; i++) {
        if (!( s[i] >= '0' && s[i] <= '9' )) return 0;
    }
    return  (s[0] == '-') ? -1 : 1 ;
}

int stringtoint (string s) {
    int ret ;
    stringstream(s) >> ret ;
    return ret ;
}
string decimaltohexa (int x) {
    std::stringstream ss;
    ss<< std::hex << x;
    std::string res ( ss.str() );
   return res;
}
string getstr(int x) {
    stringstream ss;
    ss << x;
    string str = ss.str();
    return str;
}
int isSymbol (string s ) {
  for (int i = 0 ; i < (signed)s.length() ; i++) {
        if (!( s[i] >= 'A' && s[i] <= 'Z' )) return 0;
    }
   return 1 ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////*********paaaaaasssss 2/////////////////////////////////////////////////////////
char isIndexed(string operand){
                        if((operand[(operand.length())-1]=='X') && (operand[(operand.length())-2]==',')){
                                return '1';
                        }
                        return '0';
                }

                char isImmediate(string operand){
                        if (operand.at(0)!='@'){
                                return '1';
                }
                        return '0';
                }

                char isIndirect(string operand){
                        if( operand.at(0)=='#'){
                                return '0';
                        }
                        return '1';
                }

                char isBase(string operand,int displacment){
                        if(baseRelative==true){
                 if(operand[0]=='#' && isdigit(operand[1]) || operand[0]=='@' && isdigit(operand[1]) || isdigit(operand[0]) || operand[0]=='*'){
                    return '0';
                 }
                                if( displacment > 2047 && displacment <= 4096 ){
                                        return '1';
                                }
                                return '0';
                        }

                        else{ // no base definition ===> error occured
                                return '0';
                        }
                }

                char isPC (string Operand,int displacment){
                    if((Operand[0]=='#' && isdigit(Operand[1])) || (Operand[0]=='@' && isdigit(Operand[1])) || isdigit(Operand[0]) ){
                    return '0';
                 }
                        if(displacment <= 2047 && displacment >= -2048){
                                return '1';
                        }
                        if(Operand[0]=='*'){
                            return '1';
                        }
                        return '0';
                }


                char isFour(string mnemonic){
                        if(mnemonic[0]=='+'){
                                return '1';
                        }
                        return '0';
                }


                int calculateDisplacment(string operand,int indexLocation)  //to calculate the displacment of an operand
                {
                    int displacment;
            int symbolAddress=symtable[operand];
            if(operand=="SS"){

            }
            //
            string tempMn=mnemonics.at(indexLocation);
            if(mnemonics[indexLocation][0]=='+'){
                tempMn=tempMn.substr(1,tempMn.length()-1);
            }
            int opcode1=symtable[operand];
           // string opcode=decimaltohexa(opcode1);
            if( opcode1==0 && tempMn!="LDA"){
                return 0;  //sympole not found in sympole table
           }
            //

                    if(symtable.find(operand) == symtable.end())//operand not found in symbol table
            {
                        symNotFound[indexLocation]=-1;
                return 0;
            }
                    else{
              displacment=symbolAddress-AddressList[indexLocation+1];
                    }
                    if(displacment > 2047 && displacment < 4096 && baseRelative){
                        displacment=symtable[baseOperand]-AddressList[indexLocation+1];
                    }

                    if(mnemonics[indexLocation][0]=='+'){
                        if(symtable.find(operand) != symtable.end()){
                                displacment=symtable[operand];

                        }


                    }
                    return displacment;
                }

                //------------------------------------------------------------------------------------
                string opCodeGeneration(string mnemonic,string operand,int i)
                {
                     string tempMnemonic=mnemonic;
            if(mnemonic[0]=='+'){
             mod.push_back(i);
            tempMnemonic=mnemonic.substr(1,mnemonic.length()-1); // needed to be reformulated lama el net yerga3 :'(
            }
            int opcode1=optable[tempMnemonic];
            string opcode=decimaltohexa(opcode1);
            if( opcode1==0 && tempMnemonic!="LDA"){
                return "null";  //sympole not found in sympole table
           }
            if(opcode.length()==1){
                opcode="0"+opcode;
            }
                string s=opcode.substr(1,1);
                //cout<<"this is pocode"<< opcode << endl;
                s = toUpper(s);
                cout<<"this is s"<<s<<endl ;
            string opToBinary=GetBinaryStringFromHexString(s);
            cout<< "this is op"<<opToBinary<<" "<<endl ;
                    string first2Bits=opToBinary.substr(0,2);
                    string flagsString;
                    for(int j=0;j<6;j++){
                flagsString+=flags[i][j];
                    }


                    int operandTyp=operandType(operand);

                    if(operandTyp==1){

                        int absolute = 0;

                         int found [4];
                         found[0] = operand.find('+');
                         found[1] = operand.find('-');
                         found[2] = operand.find('*');
                         found[3] = operand.find('/');

                         int pos = found[0] ;
                         int op = 0 ;
                         for(int i = 1 ; i < 4 ; i++){
                                if(pos != -1){
                                        break ;
                                }
                                pos = found[i];
                                op = i ;
                         }

                         string sub1 = operand.substr(0,pos);
                         string sub2 = operand.substr(pos+1,operand.size()-(pos+1));

                         int dis1 = 0 ;
                         int dis2 = 0 ;

                         if(isalpha(sub1[0])){
                                 if(symtable.find(sub1) != symtable.end()){
                                         dis1 = symtable [sub1] ;
                                 }
                                 else{
                                         symNotFound[i] = -1;
                                         return " " ;
                                 }
                         }
                         else{
                                 istringstream buffer(sub1);

                                 buffer >> dis1;
                         }

                        ////////// sub2////////////

                         if(isalpha(sub2[0])){
                                 if(symtable.find(sub2) != symtable.end()){
                                         dis1 = symtable [sub2] ;
                                 }
                                 else{
                                         symNotFound[i] = -1 ;
                                         return " " ;
                                 }
                         }
                         else{
                                 istringstream buffer(sub2);

                                 buffer >> dis2;
                         }

                         ///  calculate absolute////

                         if(op == 0){
                                 absolute = dis1 + dis2 ;
                         }
                         else if(op == 1){
                                 absolute = dis1 - dis2 ;
                         }
                         else if(op == 2){
                                 absolute = dis1 * dis2 ;
                         }
                         else if(op  == 4 ){
                                 absolute = dis1 / dis2 ;
                         }

                         string ab = decimaltohexa(absolute) ;


                         string total=first2Bits+flagsString;
                            total=GetHexFromBin(total);

                            if(ab.length()<3){
                                if(ab.length()<2){
                                    total=total+"00";
                                }
                                else{
                                total=total+"0";
                                }
                    }

                            string result=opcode.at(0)+total+ab;
                            return result;



                    }
                    if(operandTyp == 2){
                string Rone = operand.substr(0,1);
                string Rtwo = operand.substr(2,1);
                int r1 = regtable[Rone];
                int r2 = regtable[Rtwo];

                Rone = decimaltohexa(r1);
                Rtwo = decimaltohexa(r2);

                string total = opcode+Rone+Rtwo ;
                return total ;
                    }

                    else if(operandTyp == 13){
                int add = AddressList[i] ;
                string address = decimaltohexa(add);

                string total=first2Bits+flagsString;
                    total=GetHexFromBin(total);

                    if(address.length()<3){
                        if(address.length()<2){
                            total=total+"00";
                        }
                        else{
                        total=total+"0";
                        }
            }
                    string result=opcode.at(0)+total+address;
                    return result;

                    }

                    else if(operandTyp == 3 || operandTyp == 4 || operandTyp == 5 || operandTyp == 6){
                int value;
                string newop ;
                if(operandTyp == 3){
                    newop = operand.substr(0,operand.find(','));
                }
                else if(operandTyp == 4){
                    newop = operand ;
                }
                else if(operandTyp == 5 || operandTyp == 6){
                    newop = operand.substr(1,operand.length()-1);
                }

                        istringstream buffer(newop);
                        buffer >> value;

                        string newvalue = decimaltohexa(value);

                        string total=first2Bits+flagsString;
                    total=GetHexFromBin(total);

                    if(newvalue.length()<3){
                        if(newvalue.length()<2){
                            total=total+"00";
                        }
                        else{
                        total=total+"0";
                        }
            }

                    string result=opcode.at(0)+total+newvalue;
                    return result;

                    }




                    else if(operandTyp == 7 || operandTyp ==8 || operandTyp == 9 || operandTyp == 10 || operandTyp == 11 || operandTyp == 12){

                            int displ=calculateDisplacment(parseOperand(operand),i);
                            if(displ==0){
                    symNotFound[i]=-1;
                                return " ";
                            }
                    ostringstream convert;
                    convert << displ;
                    string  disp= convert.str();
                    cout<<mnemonics.at(i)<<" "<< first2Bits<<" "<< flagsString<<endl ;
                    string total=first2Bits+flagsString;
                    cout<<total<<endl ;
                    total=GetHexFromBin(total);
                   // cout<<"displacment"+disp<<endl;
                    disp=decimaltohexa(displ);
                    if(disp.length()<3){
                        if(disp.length()<2){
                            total=total+"00";
                        }
                        else{
                        total=total+"0";
                        }
                }
                if(disp.length()>5){
                    disp=disp.substr(5,3);

                }
                if(isFour(mnemonics.at(i))=='1'){
                    while(disp.length()<5){
                        disp="0"+disp;
                    }
                }



                    string result=opcode.at(0)+total+disp;
                    return result;
                    }
                }

     //-------------------------------------------------------------------------------------------
        string GetBinaryStringFromHexString (string sHex)
                {
                        string sReturn = "";
                        for (int i = 0; i < sHex.length (); ++i)
                        {
                                switch (sHex [i])
                                {
                                        case '0': sReturn.append ("0000"); break;
                                        case '1': sReturn.append ("0001"); break;
                                        case '2': sReturn.append ("0010"); break;
                                        case '3': sReturn.append ("0011"); break;
                                        case '4': sReturn.append ("0100"); break;
                                        case '5': sReturn.append ("0101"); break;
                                        case '6': sReturn.append ("0110"); break;
                                        case '7': sReturn.append ("0111"); break;
                                        case '8': sReturn.append ("1000"); break;
                                        case '9': sReturn.append ("1001"); break;
                                        case 'A': sReturn.append ("1010"); break;
                                        case 'B': sReturn.append ("1011"); break;
                                        case 'C': sReturn.append ("1100"); break;
                                        case 'D': sReturn.append ("1101"); break;
                                        case 'E': sReturn.append ("1110"); break;
                                        case 'F': sReturn.append ("1111"); break;
                                }
                        }

                //      cout<<sReturn;

                        return sReturn;
                }
    //----------------------------------------------------------------------------------------------------
   string GetHexFromBin(string sBinary)
                {
                        string rest = "" ;

                        for (int i=0;i<sBinary.length();i+=4)
                        {
                                string tmp = sBinary.substr(i,4);
                                if (tmp.compare("0000") == 0)
                                {
                                        rest = rest + "0";
                                }
                                else if (tmp.compare("0001") == 0)
                                {
                                        rest = rest + "1";
                                }
                                else if (tmp.compare("0010") == 0)
                                {
                                        rest = rest + "2";
                                }
                                else if (tmp.compare("0011") == 0)
                                {
                                        rest = rest + "3";
                                }
                                else if (tmp.compare("0100") == 0)
                                {
                                        rest = rest + "4";
                                }
                                else if (tmp.compare("0101") == 0)
                                {
                                        rest = rest + "5";
                                }
                                else if (tmp.compare("0110")== 0)
                                {
                                        rest = rest + "6";
                                }
                                else if (tmp.compare("0111") == 0)
                                {
                                        rest = rest + "7";
                                }
                                else if (tmp.compare("1000") == 0)
                                {
                                        rest = rest + "8";
                                }
                                else if (tmp.compare("1001") == 0)
                                {
                                        rest = rest + "9";
                                }
                                else if (tmp.compare("1010") == 0)
                                {
                                        rest = rest + "A";
                                }
                                else if (tmp.compare("1011") == 0)
                                {
                                        rest = rest + "B";
                                }
                                else if (tmp.compare("1100") == 0)
                                {
                                        rest = rest + "C";
                                }
                                else if (tmp.compare("1101") == 0)
                                {
                                        rest = rest + "D";
                                }
                                else if (tmp.compare("1110") == 0)
                                {
                                        rest = rest + "E";
                                }
                                else if (tmp.compare("1111") == 0)
                                {
                                        rest = rest + "F";
                                }
                                else
                                {
                                        continue;
                                }
                        }
                        return rest;
   }
   //---------------------------------------------------GET operand type-----------------------------------------------
   int operandType(string op){

       // return 1 if simple expression .... return 2 if Register,register  .... return 3 if has symbol ... return 4 if * ... return 5 if space
       int found0  = op.find('+');
       int found1 = op.find('-');
       int found2 = op.find('*');
       int found3 = op.find('/');
        if(found0 != -1 || found1 != -1 || ((found2 != -1) && (found2 != 0))  || found3 != -1){
           return 1 ;
       }
        else if(op[1] == ',' && op.size() == 3){
           return 2 ;
       }
       else if(isdigit(op[0]) || isalpha(op[0]) || op[0] == '#' || op[0] == '@'){

        if(isdigit(op[0])){
                if(op.find(',') != -1){
                        return 3 ;

                }
                else{
                        return 4 ;
        }
        }
        else if(op[0] == '#' && isdigit(op[1])){
                return 5 ;
        }
        else if(op[0] == '@' && isdigit(op[1])){
                return 6 ;
        }

        else if(isalpha(op[0])){
                if(op.find(',') != -1){
                        return 7 ;

                }
                else {
                        return 8 ;
                }

        }

        else if(op[0] == '#' && isalpha(op[1])){
                if(op.find(',') != -1){
                        return 9 ;

                }
                else {
                        return 10 ;
                }
        }
        else if(op[0] == '@' && isalpha(op[1])){
                if(op.find(',') != -1){
                        return 11 ;

                }
                else {
                        return 12 ;
                }
        }

       }


       else if(op[0] == '*'){
           return 13 ;
       }
      else if(op[0] == ' '){
           return 14 ;
       }

   }
   //-------------------------------------------parse operand--------------------------------------------------------
   string parseOperand(string op){
       if(op[0] == '@' || op[0] == '#'){
           if(isdigit(op[1])){
               return "NULL" ;
           }
           else{
               int found  = op.find(',');
               if(found != -1){
                   return op.substr(1,found-1);
               }
               else{
                   return op.substr(1,op.size()-1);
               }
           }
       }

       else{
           if(isdigit(op[0])){
               return "NULL" ;
           }
           else{
               int found  = op.find(',');
               if(found != -1){
                   return op.substr(0,found);
               }
               else{
                   return op.substr(0,op.size());
               }
           }
       }

  }
  //------------------------------------------------------------pass 2 algorithm-----------------------------------
   string pass2(){
            int i = 0 ;
            while(mnemonics.at(i) != "END"){
                if(mnemonics.at(i) == "START" || mnemonics.at(i) == " " ){
                    objectCode.push_back(" ");
                }
//                else if(label[i][0] == '.'){
//                      ObjectCode.push_back(" ");
//                }
                else if(mnemonics.at(i) == "BASE" || mnemonics.at(i)=="LDB"){
                    if(mnemonics.at(i)=="LDB"){
                        testflags(i);
                    }
                        if(mnemonics.at(i)=="BASE"){
                                baseOperand=operand.at(i);
                                objectCode.push_back(" ");
                        }
                        if(mnemonics.at(i)=="LDB"){
                                ldBaseOperand=operand.at(i);
                                testflags(i);
                                objectCode.push_back(opCodeGeneration(mnemonics.at(i),operand.at(i),i));

                        }
                        if(baseOperand==("#"+ldBaseOperand)){
                                if(baseOperand == "*"){
                                        baseOperand=AddressList[i];

                                }
                                baseRelative=true;

                        }
                }

                else{
                    string temp=mnemonics.at(i);
                    if(temp[0]=='+'){
                        temp=temp.substr(1,temp.length()-1);
                    }
                    if(optable.find(mnemonics.at(i)) != optable.end() || optable.find(temp) != optable.end()){
                           if(operandType(operand.at(i)) != 2){
                                 testflags(i);
                           }

                         objectCode.push_back(opCodeGeneration(mnemonics.at(i),operand.at(i),i));
                    }
                    else if (mnemonics.at(i) == "BYTE"){

                        string op=operand.at(i);
                        int counter=0;
                        string operand1;

                        for (int j = 0; j < op.length() && counter<2; j++) {
                                if(operand[i][j]=='\''){
                                                counter++;
                                                continue;
                                        }
                                if(counter>0 && counter<2){
                                        operand1+=operand[i][j];

                                }

                                                }
                        if(operand[i][0]=='X'){
                         objectCode.push_back(operand1);
                        }

                        else if(operand[i][0]=='C'){

                                string operand2="";
                                for(int z=0;z<operand1.length();z++){
                                        int asci=(int)operand1[z];
                                        operand2=operand2+decimaltohexa (asci);
                                }
                                objectCode.push_back(operand2);
                        }

                    }
                    else if(mnemonics.at(i)=="WORD"){
                        string operand1=operand.at(i);
                        int opcode2;
                        istringstream buffer(operand1);
                        buffer >> opcode2;
                        string lengthCheck=decimaltohexa(opcode2);
                        if(lengthCheck.length()>6){
                                int length=lengthCheck.length();
                                lengthCheck=lengthCheck.substr(length-6, 6);
                        }
                        objectCode.push_back(lengthCheck);

                    }
                    else{

                        objectCode.push_back(" ");
                    }



                }
                i++;

            }


            return printPass2();
    }
    //-----------------------------------test flags-----------------------------------------------------------------
     void testflags( int i){
       flags[i][0]=isIndirect(operand.at(i));
       flags[i][1]=isImmediate(operand.at(i));
       flags[i][2]=isIndexed(operand.at(i));
       flags[i][3]=isBase(operand.at(i),calculateDisplacment(operand.at(i),i));
       flags[i][4]=isPC(operand.at(i),calculateDisplacment(operand.at(i),i));
       flags[i][5]=isFour(mnemonics.at(i));

}
//------------------------------------------------- print pass 2-----------------------------------------------------
string printPass2(){
           string ans1 = "" ;
           ans1+= ">>   S t a r t   o f    P a s s   I I \n\n" ;
           ans1+= ">>   A s s e m b l e d    p r o g r a m     l i s t i n g \n\n" ;
           ans1 += " LC    Code         Source Statement\n\n" ;

           bool pass2Success=true;
           int lenflag = 0 ;

//         ans +=addressList[i]+"        "+label.at(i)+"      "+operand.at(i)+"\n\n";
           ////////////////////////////////////////////////////////////////////////////////////////////
      /*  for (int i=0 ; i <(signed)startcomment.size();i++ ){
            ans1+="           ";
            ans1 += startcomment[i] ;
                    ans1+='\n';
                   }*/
                while (  objectCode.size() < label.size() ) objectCode.push_back(" ");
                   cout << label.size() << " "<< mnemonics.size() << " " << operand.size() << " " << comment.size()<<" " << objectCode.size() << endl;
                   for (int i=0;i<(signed)label.size()-1;i++) {
                           lenflag = 0 ;

                           if(objectCode.at(i)!=" "){
                                   if(mnemonics.at(i)!="BYTE" && mnemonics.at(i)!="WORD"&&mnemonics.at(i)!="RESW"&&mnemonics.at(i)!="RESB" && operandType(operand.at(i)) != 2 ){
                    ans1+="                             ";
                    ans1+=  "reloc   ";
                    ans1 +="n=";
                    ans1+= flags[i][0];
                    ans1+= " i=";
                    ans1 += flags[i][1];
                    ans1+= " x=";
                    ans1+=flags[i][2] ;   ans1+="   b="; ans1+=flags[i][3]; ans1+=" p="; ans1+=flags[i][4]; ans1+=" e="; ans1+=flags[i][5] ; ans1+='\n';

                                   }
                           }


                           if(symNotFound[i] == -1){
                                   pass2Success=false;
                                   ans1+=" ****** undefined symbol in operand \n" ;
                           }

                         if ( label [i] == " " && mnemonics[i] == " " && operand [i] == " " && comment [i] == " "  )  {

                                        ans1+= '\n' ;
                                continue ;
                            }
                    if (label [i][0] == '.') {
                        ans1+="           ";
                        ans1+= label [i] ;
                        ans1+= '\n';
                        continue ;
                    }
                    string address = toUpper(decimaltohexa (AddressList[i])) ;
                    for (int j=0 ; j < 6-((signed)address.length()) ;j++) {
                        ans1+='0' ;
                    }
                    ans1 += address ;
                    //cout << ans << endl;
                    ans1 += ' ' ;


                    string obcode=objectCode.at(i) ;
                    //ans +=objectCode.at(i);
                           if(obcode.size() <= 6){
                                   for (int j=0 ; j < 6-obcode.size() ;j++) {
                                       obcode+=' ' ;
                                   }
                                   ans1+=obcode;
                           }
                           if(obcode.size()>6){
                                   ans1+= obcode.substr(0,6);
                                   lenflag = 1 ;

                           }


                    ans1 += ' ' ;
                    ans1 += label [i] ;
                    for (int j=0 ; j < 9-((signed)label[i].length()) ;j++) {
                        ans1+=' ' ;
                    }
                    ans1 += mnemonics[i] ;
                    for (int j=0 ; j < 8-((signed)mnemonics[i].length()) ;j++) {
                        ans1+=' ' ;
                    }
                    ans1+=operand [i];
                    for (int j=0 ; j < 18-((signed)operand[i].length()) ;j++) {
                        ans1+=' ' ;
                    }
                    for (int j=0 ; j < 31 ;j++) {
                        if (j>= (signed)comment[i].length()) break ;
                        ans1+= comment[i][j] ;
                    }
                    ans1 += '\n' ;

                    //if(symNotFound[i] == -1){
                        //         ans1+=" ****** undefined symbol in operand \n" ;
                         //  }

                    if(lenflag == 1){
                        int s = obcode.size() - 6 ;
                        int currentPos=6;
                        while(s > 0){
                                if(s > 6 ){

                                        ans1+="      "+obcode.substr(currentPos,6)+'\n';
                        currentPos+=6;
                                }
                                else{
                                        ans1+="      "+obcode.substr(currentPos,s)+'\n';
                                        currentPos+=s;
                                }
                                s = s-6 ;
                        }
                    }
                    if(symNotFound[i] == -1){
                                   pass2Success=false;
                                   ans1+=" ****** undefined symbol in operand \n" ;
                           }
                   }
              /*      for (int i=0 ; i <(signed)endcomment.size();i++ ){
                        ans1+="           ";
                        ans1 += endcomment[i] ;
                ans1+='\n';

            }*/

                    if (pass2Success == true) {
                        ans1 += ">>    e n d    o f   p a s s   2\n\n" ;
                        ans1+= ">>   *****************************************************\n" ;
                    }
                    else{
                        ans1+=">>    i n c o p l e t e    a s s e m b l y";
                    }
                    return ans1;
           //////////////////////////////////////////////////////////////////////////////////////
   }
