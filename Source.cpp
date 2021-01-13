// Politehnièki Fakultet Zenica 
// Softversko Inženjerstvo 2.Godina
// Objektno-orijentisano programiranje
// Grupa9: Lamija Imamoviæ, Adin Jahiæ, Muharem Keleštura, Elmin Marevac
// Naziv projekta: Breakout Game (c++)
// V1.0 Made by: Adin Jahic, Lamija Imamovic (3.1.2012)
// Copyright ©2021
//--------------------------------------------------------------------------
// Napravljen prototip igre, kod nije struktuiran, oèekuje se dorada od ostalih èlanova po dogovoru
// prilikom izdavanja V2.0,(sistem bodovanja,ispisa uspjeha na fajl dodatne modifikacije i uklanjanje glitcheva).
// Dokumentacija u izradi,korišteni komentari u ovoj verziji.


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std;
using namespace sf;

const int sirinaEkrana = 800;
const int visinaEkrana = 600;
const float velicinaLopte = 10.0;
const float brzinaLopte = 6.0;
const float sirinaReketa = 100.0; 
const float visinaReketa = 20.0; 
const float brzinaReketa = 8.0;
const float sirinaBloka = 60.0;
const float visinaBloka = 20.0;
const int brojBlokovaX = 11;
const int brojBlokovaY = 6;
bool radi = true;


struct Lopta
{
    CircleShape oblik;
    Vector2f vektorBrzineLopte{ -brzinaLopte, -brzinaLopte };

    float x() { return oblik.getPosition().x; }
    float y() { return oblik.getPosition().y; }
    float lijevaStrana() { return x() - oblik.getRadius(); }
    float desnaStrana() { return x() + oblik.getRadius(); }
    float gornjaStrana() { return y() - oblik.getRadius(); }
    float donjaStrana() { return y() + oblik.getRadius(); }

    Lopta(float mX, float mY)
    {
        oblik.setPosition(mX, mY);
        oblik.setRadius(velicinaLopte);
        oblik.setFillColor(Color::White);
        oblik.setOrigin(velicinaLopte, velicinaLopte);
    }

    void update()
    {
        //DIO ZA odbijanje Lopte od rubove ekrana
        oblik.move(vektorBrzineLopte);
        //ako preleti ulijevo obrcemo mu brzinu 
        if (lijevaStrana() < 0)
            vektorBrzineLopte.x = brzinaLopte;
        //isti fazon za desno
        else if (desnaStrana() > sirinaEkrana)
            vektorBrzineLopte.x = -brzinaLopte;
        //isti fazon za gore
        if (gornjaStrana() < 0)
            vektorBrzineLopte.y = brzinaLopte;
        //u slucaju ako je preleti dole zelimo prekinut igru jer je to kriterijum
        else if (donjaStrana() > visinaEkrana)
            radi = false;
    }

};

//pravimo strukturu praouganoik za cigle koje treba razbit
struct basicBlok
{
    RectangleShape oblik;
    float x() { return oblik.getPosition().x; }
    float y() { return oblik.getPosition().y; }
    float lijevaStrana() { return x() - oblik.getSize().x / 2.f; }
    float desnaStrana() { return x() + oblik.getSize().x / 2.f; }
    float gornjaStrana() { return y() - oblik.getSize().y / 2.f; }
    float donjaStrana() { return y() + oblik.getSize().y / 2.f; }
};

//Posto je onaj glavni dio koji fata loptu on predstavlja specijalni slucaj kada cigli pa je nasljedjen
struct Reket : public basicBlok
{
    //pravimo brzinu kretnje 
    Vector2f vektorBrzineReketa;
    //formiramo konstruktorom sve sto treba da napravimo tu glavnu ciglu 
    Reket(float mX, float mY)
    {
        oblik.setPosition(mX, mY);
        oblik.setSize({ sirinaReketa, visinaReketa });
        oblik.setFillColor(Color::Blue);
        oblik.setOrigin(sirinaReketa / 2.f, visinaReketa / 2.f);
    }
    // pvdje gledamo kako se mice i dodajemo koliziju
    void update()
    {
        oblik.move(vektorBrzineReketa);
        //moramo rijesiti slucaje kada preleti glavna cigla lijevo i desno
        //kada preleti izvan okvira ekrana obrcemu mu brinu cineci nemoguce da izadje izvan ekrana 
        if (Keyboard::isKeyPressed(Keyboard::Key::Left) && lijevaStrana() > 0)
            vektorBrzineReketa.x = -brzinaReketa;

        else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && desnaStrana() < sirinaEkrana)
            vektorBrzineReketa.x = brzinaReketa;
        //logicno ako ne micemo ciglu bice brzina 0 LOGICNO
        else
            vektorBrzineReketa.x = 0;
    }
};
// eh sad moramo napravit cigle koje razbijamo
struct Blok : public basicBlok
{
    bool pogodjena = false;
    //isti fazon konstruktorom pravimo sve cigle koje su potrebne za razbit loptom
    Blok(float mX, float mY)
    {
        oblik.setPosition(mX, mY);
        oblik.setSize({ sirinaBloka, visinaBloka });
        oblik.setFillColor(Color::Black);
        oblik.setOrigin(sirinaBloka / 2.f, visinaBloka / 2.f);
    }
};

//koristeci generike u c++ programskom jeziku cemo rijesiti probleme kolizije jer imamo
//2 slucaja kolizije, prvi je izmedju lopte i glavne cigle i lopte i ostalih cigli
template <class T1, class T2>
bool dirajulSe(T1& mA, T2& mB)
{
    //ako se koordinate poklope logicno je da se desio dodir
    return mA.desnaStrana() >= mB.lijevaStrana() && mA.lijevaStrana() <= mB.desnaStrana() &&
        mA.donjaStrana() >= mB.gornjaStrana() && mA.gornjaStrana() <= mB.donjaStrana();
}

void collisionTest(Reket& mPaddle, Lopta& mBall)
{
    if (!dirajulSe(mPaddle, mBall)) return;

    mBall.vektorBrzineLopte.y = -brzinaLopte;
    if (mBall.x() < mPaddle.x())
        mBall.vektorBrzineLopte.x = -brzinaLopte;
    else
        mBall.vektorBrzineLopte.x = brzinaLopte;
}


void collisionTest(Blok& mBrick, Lopta& mBall)
{
    if (!dirajulSe(mBrick, mBall)) return;
    mBrick.pogodjena = true;

    float kontaktLijevo = mBall.desnaStrana() - mBrick.lijevaStrana();
    float kontaktDesno = mBrick.desnaStrana() - mBall.lijevaStrana();
    float kontaktGore = mBall.donjaStrana() - mBrick.gornjaStrana();
    float kontaktDole = mBrick.donjaStrana() - mBall.gornjaStrana();

    bool dodirLijevo = abs(kontaktLijevo) < abs(kontaktDesno);
    bool dodirGore = abs(kontaktGore) < abs(kontaktDole);

    float dodirX = dodirLijevo ? kontaktLijevo : kontaktDesno ;
    float dodirY = dodirGore ? kontaktGore : kontaktDole;

    if (abs(dodirX) < abs(dodirY))
        mBall.vektorBrzineLopte.x = dodirLijevo ? -brzinaLopte : brzinaLopte;
    else
        mBall.vektorBrzineLopte.y = dodirGore ? -brzinaLopte : brzinaLopte;
}

void reset() {
    Lopta ball{ sirinaEkrana / 2, visinaEkrana / 2 };
    vector<Blok> blokovi;
    for (int iX = 1;iX <=brojBlokovaX; iX++)
        for (int iY = 1;iY <=brojBlokovaY; iY++)
            blokovi.emplace_back(
                iX * (sirinaBloka + 3) + 22, (iY + 1)* (visinaBloka + 3));
}

int main()
{

    //We render/create the window
    RenderWindow window(VideoMode(sirinaEkrana, visinaEkrana), "Grupa9 (OOP), BreakoutGame");
    window.setFramerateLimit(60);

    Reket paddle(sirinaEkrana / 2, visinaEkrana - 20);
    //prvi put da mi je u zivotu pala ideja da koristim GOTO naredbu jer ovo je mislim rijetka situacija
    //pravimo loptu i cigle
restart:
    radi = true;
    Lopta ball(sirinaEkrana / 2, visinaEkrana / 2);
    vector<Blok> blokovi;
    for (int iX = 0; iX < brojBlokovaX; ++iX)
        for (int iY = 0; iY < brojBlokovaY; ++iY)
            blokovi.emplace_back(
                (iX + 1) * (sirinaBloka + 3) + 22, (iY + 2) * (visinaBloka + 3));


    while (true) // ovo je glavna petlja igre
    {
        window.clear(Color::Color(49, 79, 79));

        if (Keyboard::isKeyPressed(Keyboard::Key::Space))
            goto restart;
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
            break;

        ball.update();
        paddle.update();
        collisionTest(paddle, ball);
        for (auto& blok : blokovi) collisionTest(blok, ball);
        blokovi.erase(remove_if(begin(blokovi), end(blokovi),
            [](const Blok& mBrick)
            {
                return mBrick.pogodjena;
            }),
            end(blokovi));

        if (radi)
        {
            window.draw(ball.oblik);
            window.draw(paddle.oblik);
            for (auto& blok : blokovi) window.draw(blok.oblik);
        }

        else
                goto restart;

        

        window.display();

    }

}


