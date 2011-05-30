/*
 * MineCrisis
 * Copyright (C) 2010-2011 Giacomo Drago
 * http://giacomodrago.com/go/minecrisis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/***********************************************************
* ASE_Object (era: Ase3dObject class)
*
* Descrizione:
* importa la geometria di un oggetto 3d memorizzato in formato ASE (Ascii Extended)
* un oggetto può essere formato a sua volta da più oggetti 3d indivisibili (primitive geometriche o mesh)
* La geometria di una mesh è formata da:
*  - coordinate dei vertici
*  - topologia delle facce, cioè come le facce sono connesse (es. (1, 4, 7)
*  indica una faccia composta dai vertici 1, 4 e 7)
*  - normali, cioè il vettore normale al piano definito dalla faccia.
*  Le normali sono utili per una corretta illuminazione
*             una normale può anche essere definita per un singolo vertice
*
* Modificato da Giacomo Drago (settembre 2010)
* 1) conversione da C++ a C99
* 2) aggiunta del parametro use_vertex_normals per le funzioni di disegno
* 3) cambiamenti minori
* 4) traduzione messaggi di debug in inglese
* 5) aggiunta funzione calcolo bounding box e altre funzioni
* 6) TODO: implementare coordinate texture!
*
************************************************************/

#ifndef ASE_IMPORTER_H
#define ASE_IMPORTER_H

#include <stdbool.h>
#include "bounding_boxes.h"


typedef struct {		    // definisce la struttura punto
    GLdouble x, y, z;
    GLdouble nx, ny, nz;	// normale del vertice
} vertex_t;

typedef struct {            // definisce la struttura faccia
    GLint v0, v1, v2;		// vertici componenti la faccia
    GLdouble nx, ny, nz;	// normale della faccia
} face_t;

typedef struct {
    char name[255];
    int n_vertexes;
    int n_faces;
    vertex_t* vertexes;
    face_t* faces;
} mesh_t;

typedef struct {
    int n_meshes;
    mesh_t* meshes;
    int n_vertexes;
} ASE_Object;


ASE_Object* ASE_create(const char* filename);
void ASE_destroy(ASE_Object* ase);
int ASE_numVertexes(ASE_Object* ase);
void ASE_getAllVertexes(ASE_Object* ase, vertex_t* array);
void ASE_drawMesh(ASE_Object* ase, const char* mesh_name, bool use_vertex_normals);
void ASE_draw(ASE_Object* ase, bool use_vertex_normals);
void ASE_swapYZ(ASE_Object* ase);
void ASE_scaleModel(ASE_Object* ase, double scale_factor_x, double scale_factor_y, double scale_factor_z);
bounding_box_t ASE_boundingBox(ASE_Object* ase);
void ASE_centerModel(ASE_Object* ase, double* translate_x, double* translate_y, double* translate_z);

#endif
