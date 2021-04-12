#include <iostream>
#include <string>

using namespace std;

class Card{
    protected:
        string nrCard;
        string numeDetinator;
        string data_expirare;
        int CVV;
        double credit;
        static int nrCarduri;

    public:
        Card(){
            ++nrCarduri;
        }
        Card(string nr = "0", string name = "unnamed", string exp = "1.1.1", int code = 0, double cred = 0){
            nrCard = nr;
            numeDetinator = name;
            data_expirare = exp;
            CVV = code;
            credit = cred;
            ++nrCarduri;
        }
        Card(const Card& c){
            this->nrCard = c.nrCard;
            this->numeDetinator = c.numeDetinator;
            this->data_expirare = c.data_expirare;
            this->CVV = c.CVV;
            this->credit = c.credit;
            ++nrCarduri;
        }
        ~Card(){
            cout<<"distrus Card \n";
        }
        Card& operator=(const Card& c){
            if(this!=&c){
                nrCard = c.nrCard;
                numeDetinator = c.numeDetinator;
                data_expirare = c.data_expirare;
                CVV = c.CVV;
                credit = c.credit;
            }
            return *this;
        }
        friend istream &operator>>(istream& in, Card& c){
            in>>c.nrCard;
            in>>c.numeDetinator;
            in>>c.data_expirare;
            in>>c.CVV;
            in>>c.credit;
            return in;
        }
        friend ostream &operator<<(ostream& out, const Card& c){
            out<<c.nrCard<<'\n';
            out<<c.numeDetinator<<'\n';
            out<<c.data_expirare<<'\n';
            out<<c.CVV<<'\n';
            out<<c.credit<<'\n';
            return out;
        }
        virtual void afisare(){
            cout<<"Nr.Card: "<<nrCard<<'\n'<<"Nume detinator: "<<numeDetinator<<'\n'<<"Data expirare: "<<data_expirare<<'\n'<<"CVV"<<CVV<<'\n'<<"Credit ramas:"<<credit<<'\n';
        }

        bool checkCVV(int check){
            if(CVV = check)
                return true;
            return false;
        }
        virtual void extrage(double val){
        }
};

class Card_standard : public Card{
    protected:
        int limitaExtragere;
        double comisionDepasireLimita;

    public:

        Card_standard(string nr = "0", string name = "unnamed", string exp = "1.1.1", int code = 0, double cred = 0, int lim = 0, double comis = 0) : Card(nr, name, exp, code, cred){
            limitaExtragere = lim;
            comisionDepasireLimita = comis;
        }

        Card_standard(const Card_standard& c) : Card(c){
            this->limitaExtragere = c.limitaExtragere;
            this->comisionDepasireLimita = c.comisionDepasireLimita;
        }

        ~Card_standard(){
            cout<<"distrus Card_standard \n";
        }

        Card_standard& operator=(const Card_standard& c){
            if(this!=&c){
                this->Card::operator=(c);
                limitaExtragere = c.limitaExtragere;
                comisionDepasireLimita = c.comisionDepasireLimita;
            }
            return *this;
        }
        friend istream &operator>>(istream& in, Card_standard& c){
            in>>(Card&)c;
            in>>c.limitaExtragere;
            in>>c.comisionDepasireLimita;
            return in;
        }
        friend ostream &operator<<(ostream& out, const Card_standard& c){
            out<<(Card&)c;
            out<<c.limitaExtragere<<'\n';
            out<<c.comisionDepasireLimita<<'\n';
            return out;
        }

        void extrage(double val){
            try{
                if(val <= credit){
                    if(val <= limitaExtragere){
                        credit -= val;
                    }
                    else{
                        credit -= limitaExtragere;
                        credit -= (val-limitaExtragere) + (val-limitaExtragere)*comisionDepasireLimita;
                    }
                    cout<<"Extragere efectuata; credit ramas pe card: "<<credit;
                }
                else{
                    throw(val);
                }
            }
            catch(double x){
                cout << "Extragere neefectuata, valoarea depaseste creditul cu " << x - credit << '\n';
            }
        }

        void afisare(){
            Card::afisare();
            cout<<"Limita extragere: "<<limitaExtragere<<'\n'<<"Comision la depasire: "<<comisionDepasireLimita<<'%'<<'\n';
        }
};


class Card_premium : public Card_standard{
    double cashback;

    public:

        Card_premium(string nr = "0", string name = "unnamed", string exp = "1.1.1", int code = 0, double cred = 0, int lim = 0, double comis = 0, double cbk = 0) : Card_standard(nr, name, exp, code, cred, lim, comis){
            cashback = cbk;
        }

        Card_premium(const Card_premium& c) : Card_standard(c){
            this->cashback = c.cashback;
        }

        ~Card_premium(){
            cout<<"distrus Card_premium \n";
        }

        Card_premium& operator=(const Card_premium& c){
            if(this!=&c){
                this->Card_standard::operator=(c);
                cashback = c.cashback;
            }
            return *this;
        }

        friend istream &operator>>(istream& in, Card_premium& c){
            in>>(Card_standard&)c;
            in>>c.cashback;
            return in;
        }
        friend ostream &operator<<(ostream& out, const Card_premium& c){
            out<<(Card_standard&)c;
            out<<c.cashback<<'\n';
            return out;
        }

        void extrage(double val){
            try{
                if(val <= credit){
                    if(val <= limitaExtragere){
                        credit -= val;
                    }
                    else{
                        credit -= (val + (comisionDepasireLimita/100)*val);
                    }
                    credit += (cashback/100)*val;
                    cout<<"Extragere efectuata; credit ramas pe card: "<<credit;
                }
                else{
                    throw(val);
                }
            }
            catch(double x){
                cout << "Extragere neefectuata, valoarea depaseste creditul cu " << x - credit << '\n';
            }
        }

        void afisare(){
            Card_standard::afisare();
            cout<<"Cashback: "<<cashback<<'%'<<'\n';
        }
};

int Card::nrCarduri = 0;

int main()
{
    int n, i = 0, opt, crd, chkCVV;
    bool x = true;
    string cardType;
    cout<<"---====(CREDITE POO)====---\n";
    cout<<"Programul nostru va ofera posibilitatea de a aplica pentru un numar nelimitat de carduri de credit, cu comisioane alese de dvs. (adica bani gratis!)\n";
    cout<<"Dati numarul de carduri de realizat:";
    cin>>n;

    Card **arr = new Card*[n];

    while(i < n){
        cout<<"Card"<<i+1<<'\n'<<"Tip card:";
        cin>>cardType;
        try{
            if(cardType == "standard"){
                cout<<"Datele de intrare sunt: Numar, Nume, Data expirare, Credit, Limita extragere, Comision depasire limita\n";
                arr[i] = new Card_standard();
                cin>>*dynamic_cast<Card_standard*>(arr[i]);
            }
            else if(cardType == "premium"){
                cout<<"Datele de intrare sunt: Numar, Nume, Data expirare, Credit, Limita extragere, Comision depasire limita, Cashback\n";
                arr[i] = new Card_premium();
                cin>>*dynamic_cast<Card_premium*>(arr[i]);
            }
            else throw(cardType);
        }
        catch(string error){
            cout<<"Bad card type: "<<error<<'\n';
            i--;
        }
        i++;
    }

    while(x){
        cout<<"Care este urmatoarea actiune?(1 - extragere numerar; 2 - detalii card;  0 - iesire)\n";
        cin>>opt;
        switch(opt){
        case 0:
            x = false;
            break;
        case 1:
            double suma;
            cout<<"De pe ce card doriti sa extrageti bani?";
            cin>>crd;
            cout<<"Introduceti CVV-ul pentru securitate:";
            cin>>chkCVV;
            if(arr[crd-1]->checkCVV(chkCVV)){
                cout<<"Introduceti suma de extras:";
                cin>>suma;
                arr[crd-1]->extrage(suma);
            }
            else cout<<"CVV invalid";
            break;
        case 2:
            cout<<"Care este cardul de verificat?";
            cin>>crd;
            cout<<"Introduceti CVV-ul pentru securitate:";
            cin>>chkCVV;
            if(arr[crd-1]->checkCVV(chkCVV)){
                arr[crd-1]->afisare();
            }
        }
    }
    delete arr;
    return 0;
}
