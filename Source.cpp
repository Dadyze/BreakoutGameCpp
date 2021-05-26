// Politehnièki Fakultet Zenica 
// Softversko Inženjerstvo 2.Godina
// Objektno-orijentisano programiranje
// Grupa9: Lamija Imamoviæ, Adin Jahiæ, Muharem Keleštura, Elmin Marevac
// Naziv projekta: Breakout Game (c++)

// Copyright ©2021
//--------------------------------------------------------------------------


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


struct Lopta
{
    CircleShape oblik;
    Vector2f vektorBrzineLopte{ -brzinaLopte, -brzinaLopte };

    bool izvanGranica = false;

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
        else if (donjaStrana() > visinaEkrana) {
            izvanGranica = true;

        }
            
    }

    bool daLiJeIzvanGranica()
    {
        return izvanGranica;
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

struct Runner
{
    Color bgColor = Color::Color(49, 79, 79);
    Font font;
    int highscore = 0;

    Runner()
    {
        if (!font.loadFromFile("fonts/arial.ttf"))
            std::cout << "Can't find the font file" << std::endl;
    }

    void renderStart(RenderWindow& window)
    {
        Text startText;
        startText.setFont(font);
        startText.setString("Press space to start");
        startText.setFillColor(Color::Black);
        startText.setCharacterSize(30);
        startText.setPosition(sirinaEkrana/2 - startText.getGlobalBounds().width / 2, visinaEkrana/2);

        Text hisghscoreText;
        hisghscoreText.setFont(font);
        hisghscoreText.setStyle(Text::Bold);
        hisghscoreText.setString("Highscore: " + to_string(highscore));
        hisghscoreText.setFillColor(Color::Black);
        hisghscoreText.setCharacterSize(48);
        hisghscoreText.setPosition(sirinaEkrana / 2 - hisghscoreText.getGlobalBounds().width / 2, visinaEkrana / 2 - 75);

        Text Title;
        Title.setFont(font);
        Title.setString("BREAKOUT GAME ");
        Title.setFillColor(Color::Black);
        Title.setCharacterSize(64);
        Title.setPosition(sirinaEkrana / 2 - Title.getGlobalBounds().width / 2, visinaEkrana / 2 - 170);
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                case Event::Closed:
                {
                    window.close();
                    break;
                }
                case Event::KeyReleased:
                {
                    if (event.key.code == Keyboard::Space) {
                        startGame(window);
                    }
                    break;
                }
                break;
                }
            }

            window.clear(bgColor);

            window.draw(startText);
            window.draw(hisghscoreText);
            window.draw(Title);
            window.display();
        }

    }

    void startGame(RenderWindow& window)
    {
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setFillColor(Color::White);
        scoreText.setCharacterSize(18);
        scoreText.setPosition(10, 10);

        Reket paddle(sirinaEkrana / 2, visinaEkrana - 20);
        Lopta ball(sirinaEkrana / 2, visinaEkrana / 2);
        vector<Blok> blokovi;
        for (int iX = 0; iX < brojBlokovaX; ++iX)
            for (int iY = 0; iY < brojBlokovaY; ++iY)
                blokovi.emplace_back(
                    (iX + 1) * (sirinaBloka + 3) + 22, (iY + 2) * (visinaBloka + 3));

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                case Event::Closed:
                {
                    window.close();
                    break;
                }
                case Event::KeyReleased:
                {
                    if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Space) {
                        renderStart(window);
                    }
                    break;
                }

                break;
                }
            }

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

            if (ball.daLiJeIzvanGranica())
            {
                renderStart(window);
            }

            int score = brojBlokovaX * brojBlokovaY - blokovi.size();
            
            if (score > highscore) {
                highscore = score;
            }

            scoreText.setString("Score: " + to_string(score));

            window.clear(bgColor);
            window.draw(scoreText);
            window.draw(ball.oblik);
            window.draw(paddle.oblik);
            for (auto& blok : blokovi) window.draw(blok.oblik);

            window.display();
        }

    }

    void collisionTest(Reket& mPaddle, Lopta& mBall)
    {
        if (! (mPaddle.desnaStrana() >= mBall.lijevaStrana() && mPaddle.lijevaStrana() <= mBall.desnaStrana() &&
            mPaddle.donjaStrana() >= mBall.gornjaStrana() && mPaddle.gornjaStrana() <= mBall.donjaStrana())) return;

        mBall.vektorBrzineLopte.y = -brzinaLopte;
        if (mBall.x() < mPaddle.x())
            mBall.vektorBrzineLopte.x = -brzinaLopte;
        else
            mBall.vektorBrzineLopte.x = brzinaLopte;
    }

    void collisionTest(Blok& mBrick, Lopta& mBall)
    {
        if (!(mBrick.desnaStrana() >= mBall.lijevaStrana() && mBrick.lijevaStrana() <= mBall.desnaStrana() &&
            mBrick.donjaStrana() >= mBall.gornjaStrana() && mBrick.gornjaStrana() <= mBall.donjaStrana())) return;

        mBrick.pogodjena = true;

        float kontaktLijevo = mBall.desnaStrana() - mBrick.lijevaStrana();
        float kontaktDesno = mBrick.desnaStrana() - mBall.lijevaStrana();
        float kontaktGore = mBall.donjaStrana() - mBrick.gornjaStrana();
        float kontaktDole = mBrick.donjaStrana() - mBall.gornjaStrana();

        bool dodirLijevo = abs(kontaktLijevo) < abs(kontaktDesno);
        bool dodirGore = abs(kontaktGore) < abs(kontaktDole);

        float dodirX = dodirLijevo ? kontaktLijevo : kontaktDesno;
        float dodirY = dodirGore ? kontaktGore : kontaktDole;

        if (abs(dodirX) < abs(dodirY))
            mBall.vektorBrzineLopte.x = dodirLijevo ? -brzinaLopte : brzinaLopte;
        else
            mBall.vektorBrzineLopte.y = dodirGore ? -brzinaLopte : brzinaLopte;
    }

};

int main()
{

    //We render/create the window
    RenderWindow window(VideoMode(sirinaEkrana, visinaEkrana), "Grupa9 (OOP), BreakoutGame");
    window.setFramerateLimit(60);

    Runner runner;

    // Render screen with Start button
    runner.renderStart(window);

}


