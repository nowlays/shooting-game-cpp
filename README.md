# Shooting Game C++

# YOUSFI IDRIS

## README

---

## Description du jeu

Le joueur controle un vaisseau qui peut se deplacer horizontalement et verticalement
et tirer des projectiles à l'aide de la barre espace. Les positions utilisent
des nombres complexes.

Les projectiles sont tirés verticalement (vers le haut).
Le vaisseau est positionné en bas et se deplace avec le clavier (les fleches).

Les ennemis sont en haut de la fenetre et se deplacent vers le bas.
On verifie si un projectile entre en collision avec un ennemi.

Un ennemi est representé par un rectangle,
les projectiles par un cercle (avec draw, sans images).

On utilise les Complex et les Particules.

---

## But du jeu

On controle un vaisseau, on doit tirer sur les ennemis qui tombent vers nous.

- Il faut detruire tous les ennemis
- Chaque ennemi vaut 100 points
- A 2000 points on gagne
- Si les ennemis arrivent jusqu'au vaisseau on perd

---

## Touche

- Fleches : pour se deplacer
- Barre espace : pour tirer

---

## Amelioration

- Faire differents vaisseaux (tir plus rapide, tirer plusieurs projectiles en meme temps) - non fait
- Un score qui compte les ennemis detruits par 100 et victoire a 3000 points - fait
- Des ennemis qui envoient des projectiles - non fait
- Des ennemis qui ne sont pas fixes - fait
- Des ennemis qui ont des points de vie et qui ne meurent pas en un coup - non fait

---

## Fonction-a-faire

- InitJeu - fait
- MouvementVaisseau - fait
- TirerProjectile - fait
- MouvementProjectile - fait
- MouvementVaisseau - fait
- Update - fait
- Collision - fait
- CollisionVaisseau - fait
- Draw - fait
- Draw_Image - fait
- Score - fait

---

## 1er-Semaine-10-avril

- Ajout de la procedure InitJeu ainsi que les structures principales du jeu
- Creation de la procedure MouvementVaisseau

---

## 3eme-Semaine-24-avril-rendu-final

- Ajout de la procedure TirerProjectile et MouvementProjectile
- Debut de la procedure Draw avec images pour le vaisseau et les ennemis
- Correction de la procedure TirerProjectile
- Ajout des procedures Collision et CollisionVaisseau pour gerer les collisions
- Creation de la procedure Draw_Image pour afficher les images du vaisseau, des ennemis et du fond
- La procedure MouvementEnnemi permet aux ennemis de descendre vers le vaisseau
- Si un ennemi atteint le bas de la fenetre, le joueur perd
- Mise en place du systeme de score :
  - 100 points par ennemi detruit
  - Victoire a 2000 points

---

## Reference-inspiration

- Inspiration principal :
https://apps.apple.com/us/app/galaxy-attack-alien-shooter/id1176011642

- Autre source :
  - https://www.reddit.com/r/shmups/comments/1cbv497/i_made_a_game_mockup_for_a_shootem_up_game_in/
  - print : https://perso.liris.cnrs.fr/alexandre.meyer/grapic/html/namespacegrapic_a33f4fbc99a34321912f989649797cdcd.html#a33f4fbc99a34321912f989649797cdcd
  - setKeyRepeatMode : https://perso.liris.cnrs.fr/alexandre.meyer/grapic/html/namespacegrapic_a328e76a64c78ccce639a78e042a4a489.html#a328e76a64c78ccce639a78e042a4a489
