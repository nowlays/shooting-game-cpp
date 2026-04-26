```cpp

#include<iostream>
#include<Grapic.h>
using namespace grapic;
using namespace std;


struct Complex
{
    float x;
    float y;
};


Complex make_complex(float x, float y)
{
    Complex z;
    z.x = x;
    z.y = y;
    return z;
}


Complex operator+(Complex a, Complex b)
{
    Complex z;
    z.x = a.x + b.x;
    z.y = a.y + b.y;
    return z;
}

Complex translate(Complex a, Complex b)
{
    Complex z;
    z = a + b;
    return z;
}

Complex operator-(Complex a, Complex b)
{
    Complex z;
    z.x = a.x - b.x;
    z.y = a.y - b.y;
    return z;
}

Complex operator*(float lambda, Complex b)
{
    Complex z;
    z.x = lambda * b.x;
    z.y = lambda * b.y;
    return z;
}

Complex operator*(Complex b, float lambda)
{
    return operator*(lambda,b);
}


Complex operator*(Complex a, Complex b)
{
    Complex z;
    z.x = a.x*b.x - a.y*b.y;
    z.y = a.x*b.y + a.y*b.x;
    return z;
}


float Distance(Complex V)
{
    return sqrt(V.x*V.x + V.y*V.y);
}

float Distance(Complex A, Complex B)
{
    return Distance(B-A);
}


// ------------------------- Initialisation-des-constantes -------------------------

const int DIMW = 600;

const int NB_PROJECTILE = 300;
const int NB_ENNEMI = 30;
const int PROJECTILE_VITESSE = 5;

const int VAISSEAU_TAILLE = 40;
const int ENNEMI_TAILLE = 40; // hitbox de l'ennemi
const int ENNEMI_RAYON = 20; // rayon de l'ennemi pour la collision
const int PROJECTILE_RAYON = 5; // hitbox du projectile


// ------------------------- Les-Images -------------------------


Image imgVaisseau;
Image imgEnnemi;
Image imgFond;

// ------------------------- Les-Structures -------------------------


struct Vaisseau
{
    Complex pos;
};

struct Projectile
{
    Complex pos;
    Complex vit;
    bool actif;
};

struct Ennemi
{
    Complex pos;
    bool vivant;
};

struct Jeu
{
    Vaisseau vaisseau;
    Projectile projectile[NB_PROJECTILE];
    Ennemi ennemi[NB_ENNEMI];
    int nb_projectile;
    int nb_ennemi;
};


// ------------------------- Programme-principal -------------------------

void InitJeu(Jeu &J, int nb_projectile, int nb_ennemi)
{
    imgVaisseau = image("data/images/ship2.png"); // chemin relatif des images
    imgEnnemi = image("data/images/ennemy.png");
    imgFond = image("data/images/stars.jpg");
    J.vaisseau.pos = make_complex(DIMW/2, 80); // on place le vaisseau au milleur-bas
    J.nb_projectile = nb_projectile;
    J.nb_ennemi = nb_ennemi;
    for(int i = 0; i < nb_projectile; i++)
    {
        J.projectile[i].actif = false;
        J.projectile[i].pos = make_complex(J.vaisseau.pos.x, J.vaisseau.pos.y);
        J.projectile[i].vit = make_complex(0, -PROJECTILE_VITESSE); // tire vers le haut
    }
    for(int i = 0; i < nb_ennemi; i++)
    {
        int ligne = i / 10; // c'est un int il prend la valeur entiere donc il va faire un parcourt de 10 ennemis a chaque ligne exemple : 1/10=0 ; 2/10=0 ; 9/10=0 ; 11/10=1 ; 22/10=2
        int colonne = i % 10; // il prend le reste donc il parcourt les colonnes, une fois arrivé à 10 le modulo va le ramener à 0 : 1%10=1 ; 2%10=2 ; 9%10=9; 10%10=0 ; 15%10=5 ; 27%10=7
        
        J.ennemi[i].vivant = true;
        J.ennemi[i].pos = make_complex(20 + colonne*60, DIMW-60 - ligne *60); // j'ai mis des valeurs pour les espacer et DIMW-60 pour les placer à partir du haut de la fenetre
    }   
}




void MouvementVaisseau(Jeu &J) // limite du vaisseau en horizontal et vertical
{
    if(isKeyPressed(SDLK_LEFT))
    {
        J.vaisseau.pos.x -= 5;
    }
    if(isKeyPressed(SDLK_RIGHT))
    {
        J.vaisseau.pos.x += 5;
    }
    if(isKeyPressed(SDLK_DOWN))
    {
        J.vaisseau.pos.y -= 5;
    }
    if(isKeyPressed(SDLK_UP))
    {
        J.vaisseau.pos.y += 5;
    }
}




void TirerProjectile(Jeu &J)
{
    if(isKeyPressed(SDLK_SPACE))
    {
        bool tire = false; // pour tirer un projectile un à un
        for (int i = 0; (i < J.nb_projectile) && (tire == false); i++)
        {
            if (J.projectile[i].actif == false)
            {
                J.projectile[i].pos = J.vaisseau.pos;
                J.projectile[i].vit = make_complex(0, 5);
                J.projectile[i].actif = true;
                tire = true;
            }
        }
    }
}



void MouvementProjectile(Jeu &J)
{
    for(int i = 0; i < J.nb_projectile; i++)
    {
        if(J.projectile[i].actif == true)
        {
            J.projectile[i].pos = J.projectile[i].pos + J.projectile[i].vit;
            if((J.projectile[i].pos.y < 0)||(J.projectile[i].pos.y > DIMW)) // si le projectile sort de la fenetre on l'enleve
            {
                J.projectile[i].actif = false;
            }    
        }
    }
}


void MouvementEnnemi(Jeu &J)
{
    for(int i = 0; i < J.nb_ennemi; i++)
    {
        J.ennemi[i].pos.y -= 0.4; // vitesse de la descente des ennemis
    }
    
}



void Score(Jeu &J)
{
    int score = 0;
    for (int i = 0; i < J.nb_ennemi; i++)
    {
        if (J.ennemi[i].vivant == false)
        {
            score += 100;
        }
        if(J.ennemi[i].pos.y <= 0)
        {
            color(255,0,0);
            print(DIMW/2 - 50, DIMW/2, "PERDU");
        }
    }
    color(255,255,255);
    print(30, DIMW - 30, score);

    if (score >= 2000)
    {
        color(255,255,0);
        print(DIMW/2 - 50, DIMW/2, "GAGNER !!");
    }
}



void Draw(Jeu &J)
{
    int i;
    color(255,255,255); // couleur du vaisseau
    rectangleFill(J.vaisseau.pos.x - 10, J.vaisseau.pos.y - 5, J.vaisseau.pos.x + 10, J.vaisseau.pos.y + 5);
    
    color(241, 196, 15); // couleur des projectiles
    for(i = 0; i < J.nb_projectile; i++)
    {
        if(J.projectile[i].actif == true)
        {
            circleFill(J.projectile[i].pos.x, J.projectile[i].pos.y, 5);
        }
    }

    color(208, 53, 59);
    for(i = 0; i < J.nb_ennemi; i++)
    {
        if(J.ennemi[i].vivant == true)
        {
            rectangleFill(J.ennemi[i].pos.x - 10, J.ennemi[i].pos.y - 5, J.ennemi[i].pos.x + 10, J.ennemi[i].pos.y + 5);
        }
    }
    Score(J);
}




void Draw_Image(Jeu &J)
{
    int i;
    image_draw(imgFond, 0, 0);
    image_draw(imgVaisseau, J.vaisseau.pos.x - VAISSEAU_TAILLE / 2, J.vaisseau.pos.y - VAISSEAU_TAILLE / 2);
    color(0, 200, 0);
    for (i = 0; i < J.nb_projectile; i++)
        {
        if (J.projectile[i].actif == true)
        {
            circleFill(J.projectile[i].pos.x, J.projectile[i].pos.y, 5);
        }
    }
    
    for (i = 0; i < J.nb_ennemi; i++)
    {
        if (J.ennemi[i].vivant == true)
        {
            image_draw(imgEnnemi, J.ennemi[i].pos.x - ENNEMI_TAILLE / 2, J.ennemi[i].pos.y - ENNEMI_TAILLE / 2);
        }
    }
    Score(J);
}




void Collision(Jeu &J)
{
    for (int i = 0; i < J.nb_projectile; i++)
    {
        if (J.projectile[i].actif == true)
        {
            for (int j = 0; j < J.nb_ennemi; j++)
            {
                if (J.ennemi[j].vivant == true)
                {
                    float distance = Distance(J.projectile[i].pos, J.ennemi[j].pos); // calcule de leurs distances

                    if (distance < PROJECTILE_RAYON + ENNEMI_RAYON) // si la distance est trop courte alors ca signifie qu'ils retrent en collision 
                    {
                        J.projectile[i].actif = false;
                        J.ennemi[j].vivant = false;
                    }
                }
            }
        }
    }
}




void CollisionVaisseau(Jeu &J) 
{
    if (J.vaisseau.pos.x <= 0)
    {
        J.vaisseau.pos.x = 0;
    }
    if (J.vaisseau.pos.x >= DIMW)
    {
        float delta = J.vaisseau.pos.x - DIMW;
        J.vaisseau.pos.x = DIMW - delta;
    }
    if (J.vaisseau.pos.y <= 0)
    {
        J.vaisseau.pos.y = 0;
    }
    if (J.vaisseau.pos.y >= DIMW)
    {
        float delta = J.vaisseau.pos.y - DIMW;
        J.vaisseau.pos.y = DIMW - delta;
    }
}


void Update(Jeu &J)
{
    MouvementVaisseau(J);
    TirerProjectile(J);
    MouvementProjectile(J);
    Collision(J);
    CollisionVaisseau(J);
    MouvementEnnemi(J); 
}



int main(int, char **)
{
    winInit("Shoot'em up", DIMW, DIMW);
    bool stop = false;
    setKeyRepeatMode(true); // pour que le vaisseau puisse se deplacer en continu
    Jeu jeu;
    backgroundColor(6, 50, 108);
    InitJeu(jeu, 150, 20);
    while (!stop)
    {
        winClear();
        Update(jeu);
        //Draw(jeu); // draw sans images
        Draw_Image(jeu);
        delay(30);
        stop = winDisplay();
    }
    winQuit();
    return 0;
}

```
