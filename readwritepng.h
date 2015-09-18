/**
  \file readwritepnh.h
  \brief Librairie permettant de lire/ecrire des png
  
  Librairie permettant de lire et ecrire des fichiers au format png.
  Cette librairie s'inspire de http://zarb.org/~gc/html/libpng.html
  modifiee par Yoshimasa Niwa pour le rendre plus simple.
  J'ai transforme ce code en librairie.


  \author Vincent Boyer 
  \author Email  : boyer@ai.univ-paris8.fr
  \date 09/2015
*/



#ifndef __READWRITEPNG_H__
#define __READWRITEPNG_H__

#include <png.h>

#ifdef __cplusplus 
extern "C" {
#endif
  
  /**
     \todo 
     Il serait utile que les fonctions de lecture et d'ecrire retourne un code de succes ou d'erreur.
     Actuellement la reussite ou l echec dans ses fonctions est materialise par un exit.
  */

  /** 
   \typedef ImagePNG

   Structure permettant de gerer les images 
  */

  typedef struct ImagePNG ImagePNG;
  
  struct ImagePNG{
    int width;               /**<largeur de l'image*/
    int height;              /**<hauteur de l'image*/ 
    png_byte color_type;     /**<type d'image*/ 
    png_byte bit_depth;      /**<nombre de bit de profondeur par composante*/
    png_bytep *row_pointers; /**<pointeur vers le premier octet de l'image*/
  };

  /**
  \brief lit un fichier au format png

  \param[in] filename nom du fichier
  \param[out] Image pointeur sur une structure de type \ref ImagePNG
  
  \par Exemple:
  read_png_file("sun.png", &image);
  */
  extern void read_png_file (const char * filename, ImagePNG *Image);

  /**
  \brief ecrit une image dans un fichier au format png

  \param[in] filename nom du fichier
  \param[in] Image pointeur sur une structure de type \ref ImagePNG
  
  \par Exemple:
  write_png_file("sun.png", &image);
  */
  extern void write_png_file (const char *filename, ImagePNG *Image);

#ifdef __cplusplus
} // extern C
#endif

#endif
