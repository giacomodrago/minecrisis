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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include "opengl.h"
#include "ase_importer.h"

void ASE_readVertexList(mesh_t* mesh, FILE* fd_ASE) {

    char in_data[255];	// stringa letta dal file ASE

    // posiziona il puntatore
    fscanf(fd_ASE, "%s", in_data);
    fscanf(fd_ASE, "%s", in_data);

    for (int i = 0; i < mesh->n_vertexes; i++) {

        vertex_t vert;

        fscanf(fd_ASE, "%s", in_data);  /* legge *MESH_VERTEX */
        fscanf(fd_ASE, "%s", in_data);  /* legge num del vertice */

        fscanf(fd_ASE, "%s", in_data);  /* coord x */
        vert.x = atof(in_data);
        fscanf(fd_ASE, "%s", in_data);  /* coord y */
        vert.y = atof(in_data);
        fscanf(fd_ASE, "%s", in_data);  /* coord z */
        vert.z = atof(in_data);

        mesh->vertexes[i] = vert;

    }

}

void ASE_readFaceList(mesh_t* mesh, FILE* fd_ASE) {

    int i;
    char in_data[255];	// stringa letta dal file ASE

    for (i = 0; i < mesh->n_faces; i++) {

        face_t face;

        // posiziona il puntatore
        do fscanf(fd_ASE, "%s", in_data);				// si posiziona su "*MESH_FACE"
        while (strcmp(in_data, "*MESH_FACE") != 0);

        fscanf(fd_ASE, "%s", in_data);  /* legge indice della faccia */

        fscanf(fd_ASE, "%s", in_data);
        fscanf(fd_ASE, "%s", in_data);  /* vertice 0 */
        face.v0 = atoi(in_data);
        fscanf(fd_ASE, "%s", in_data);
        fscanf(fd_ASE, "%s", in_data);  /* vertice 1 */
        face.v1 = atoi(in_data);
        fscanf(fd_ASE, "%s", in_data);
        fscanf(fd_ASE, "%s", in_data);  /* vertice 2 */
        face.v2 = atoi(in_data);

        mesh->faces[i] = face;

    }

}

void ASE_readNormalList(mesh_t* mesh, FILE* fd_ASE) {

    char in_data[255];	// stringa letta dal file ASE

    for (int i = 0; i < mesh->n_faces; i++) {

        /* legge la normale alla faccia i */
        // posiziona il puntatore
        do fscanf(fd_ASE, "%s", in_data);				// si posiziona su "*MESH_FACENORMAL"
        while (strcmp (in_data, "*MESH_FACENORMAL") != 0);
        fscanf(fd_ASE, "%s", in_data);  /* legge num della faccia */

        fscanf(fd_ASE, "%s", in_data);  /* faccia nx */
        mesh->faces[i].nx = atof(in_data);
        fscanf(fd_ASE, "%s", in_data);  /* faccia ny */
        mesh->faces[i].ny = atof(in_data);
        fscanf(fd_ASE, "%s", in_data);  /* faccia nz */
        mesh->faces[i].nz = atof(in_data);

        /* legge la normale ai vertici della faccia i */
        for (int k = 0; k < 3; k++) {
            fscanf(fd_ASE, "%s", in_data);  /* legge *MESH_VERTEXNORMAL */
            fscanf(fd_ASE, "%s", in_data);
            int j = atoi(in_data);			  /* j contiene l'indice del vertice */
            fscanf(fd_ASE, "%s", in_data);
            mesh->vertexes[j].nx = atof(in_data);		  /* vertice nx */
            fscanf(fd_ASE, "%s", in_data);
            mesh->vertexes[j].ny = atof(in_data);		  /* vertice ny */
            fscanf(fd_ASE, "%s", in_data);
            mesh->vertexes[j].nz = atof(in_data);		  /* vertice nz */
        }

    }

}

int ASE_loadMesh(mesh_t* mesh, FILE* fd_ASE) {

    char in_data[255];	// stringa letta dal file ASE

    int res;		// if res == -1 allora il file è terminato
    do res = fscanf(fd_ASE, "%s", in_data);				// si posiziona in testa alla mesh corrente
    while ((strcmp (in_data, "*GEOMOBJECT") != 0) && (res != -1));

    if (res == -1) return 0;

    do fscanf(fd_ASE, "%s", in_data);				// legge il nome della mesh
    while (strcmp (in_data, "*NODE_NAME") != 0);
    fscanf(fd_ASE, "%s", in_data);
    int len = strlen(in_data);
    int i;
    for (i = 1; i < len-1; i++) {
        mesh->name[i-1] = in_data[i];
    }
    mesh->name[i-1] = '\0';

    do fscanf(fd_ASE, "%s", in_data);				// legge il numero dei vertici
    while (strcmp (in_data, "*MESH_NUMVERTEX") != 0);
    fscanf(fd_ASE, "%s", in_data);
    mesh->n_vertexes = atoi(in_data);
    mesh->vertexes = (vertex_t*) malloc(mesh->n_vertexes * sizeof(vertex_t));

    do fscanf(fd_ASE, "%s", in_data);				// legge il numero delle facce
    while (strcmp (in_data, "*MESH_NUMFACES") != 0);
    fscanf(fd_ASE, "%s", in_data);
    mesh->n_faces = atoi(in_data);
    mesh->faces = (face_t*) malloc(mesh->n_faces * sizeof(face_t));

    // scansiona la vertex list
    ASE_readVertexList(mesh, fd_ASE);

    // scansiona la face list
    ASE_readFaceList(mesh, fd_ASE);

    // scansiona la normal list
    ASE_readNormalList(mesh, fd_ASE);

    return 1;
}

void ASE_processMesh(mesh_t* mesh, bool use_vertex_normals) {

    glBegin(GL_TRIANGLES);
    vertex_t* vertexes = mesh->vertexes;
    face_t* faces = mesh->faces;

    for (int i = 0; i < mesh->n_faces; i++) {

        if (use_vertex_normals)
            glNormal3d(vertexes[faces[i].v0].nx, vertexes[faces[i].v0].ny, vertexes[faces[i].v0].nz);
        else
            glNormal3d(faces[i].nx, faces[i].ny, faces[i].nz);

        glVertex3d(vertexes[faces[i].v0].x, vertexes[faces[i].v0].y, vertexes[faces[i].v0].z);

        if (use_vertex_normals)
            glNormal3d(vertexes[faces[i].v1].nx, vertexes[faces[i].v1].ny, vertexes[faces[i].v1].nz);
        glVertex3d(vertexes[faces[i].v1].x, vertexes[faces[i].v1].y, vertexes[faces[i].v1].z);

        if (use_vertex_normals)
            glNormal3d(vertexes[faces[i].v2].nx, vertexes[faces[i].v2].ny, vertexes[faces[i].v2].nz);
        glVertex3d(vertexes[faces[i].v2].x, vertexes[faces[i].v2].y, vertexes[faces[i].v2].z);
    }

    glEnd();

}

ASE_Object* ASE_create(const char* filename) {

    ASE_Object* ase = (ASE_Object*) malloc(sizeof(ASE_Object));

    // verifica l' esistenza dell file ASE e ne inizializza il puntatore
    FILE* fd_ASE = fopen(filename, "r");
    if (fd_ASE == NULL) {
        printf("Could not open '%s'.\n", filename);
        exit (-1);
    }

    int allocated_meshes = 10;
    ase->meshes = (mesh_t*)malloc(allocated_meshes * sizeof(mesh_t));
    ase->n_meshes = 0;

    while (true) {
        mesh_t mesh;
        if (ASE_loadMesh(&mesh, fd_ASE) == 0)
            break;
        if (ase->n_meshes == allocated_meshes) {
            ase->meshes = (mesh_t*)malloc((allocated_meshes+=10) * sizeof(mesh_t));
        }
        ase->meshes[ase->n_meshes] = mesh;
        ase->n_meshes++;
    }
    ase->meshes = (mesh_t*)realloc(ase->meshes, ase->n_meshes * sizeof(mesh_t));

    fclose(fd_ASE);

    printf("Model in '%s' contains %d mesh(es):\n", filename, ase->n_meshes);

    ase->n_vertexes = 0;
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        printf("\t-> %s (%d vertexes, %d faces)\n",
               mesh_curr->name, mesh_curr->n_vertexes, mesh_curr->n_faces);
        ase->n_vertexes += mesh_curr->n_vertexes;
    }
    printf("\n");

    return ase;
}

void ASE_destroy(ASE_Object* ase) {
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        free(mesh_curr->vertexes);
        free(mesh_curr->faces);
    }
    free(ase->meshes);
    free(ase);
}

int ASE_numVertexes(ASE_Object* ase) {
    return ase->n_vertexes;
}

void ASE_getAllVertexes(ASE_Object* ase, vertex_t* array) {
    int k = 0;
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        vertex_t* vertexes = mesh_curr->vertexes;
        for (int j = 0; j < mesh_curr->n_vertexes; j++) {
            vertex_t vertex_curr = vertexes[j];
            array[k++] = vertex_curr;
        }
    }
}

void ASE_drawMesh(ASE_Object* ase, const char* mesh_name, bool use_vertex_normals) {
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        if (strcmp(mesh_name,mesh_curr->name) == 0) {
            ASE_processMesh(mesh_curr, use_vertex_normals);
            break;
        }
    }
}

void ASE_draw(ASE_Object* ase, bool use_vertex_normals) {
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        ASE_processMesh(mesh_curr, use_vertex_normals);
    }
}

void ASE_swapYZ(ASE_Object* ase) {
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        vertex_t* vertexes = mesh_curr->vertexes;
        for (int j = 0; j < mesh_curr->n_vertexes; j++) {
            vertex_t* vertex_curr = &vertexes[j];
            double temp = vertex_curr->y;
            vertex_curr->y = vertex_curr->z;
            vertex_curr->z = temp;
        }
    }
}

bounding_box_t ASE_boundingBox(ASE_Object* ase) {
    bounding_box_t box;
    box.x_min = box.y_min = box.z_min = DBL_MAX;
    box.x_max = box.y_max = box.z_max = DBL_MIN;
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        vertex_t* vertexes = mesh_curr->vertexes;
        for (int j = 0; j < mesh_curr->n_vertexes; j++) {
            vertex_t* vertex_curr = &vertexes[j];
            if (vertex_curr->x > box.x_max) box.x_max = vertex_curr->x;
            else if (vertex_curr->x < box.x_min) box.x_min = vertex_curr->x;
            if (vertex_curr->y > box.y_max) box.y_max = vertex_curr->y;
            else if (vertex_curr->y < box.y_min) box.y_min = vertex_curr->y;
            if (vertex_curr->z > box.z_max) box.z_max = vertex_curr->z;
            else if (vertex_curr->z < box.z_min) box.z_min = vertex_curr->z;
        }
    }
    return box;
}

void ASE_scaleModel(ASE_Object* ase, double scale_factor_x, double scale_factor_y, double scale_factor_z) {
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        vertex_t* vertexes = mesh_curr->vertexes;
        for (int j = 0; j < mesh_curr->n_vertexes; j++) {
            vertex_t* vertex_curr = &vertexes[j];
            vertex_curr->x *= scale_factor_x;
            vertex_curr->y *= scale_factor_y;
            vertex_curr->z *= scale_factor_z;
        }
    }
}

void ASE_centerModel(ASE_Object* ase, double* translate_x, double* translate_y, double* translate_z) {
    bounding_box_t box = ASE_boundingBox(ase);
    double offset_x = -(box.x_max + box.x_min) / 2.0;
    double offset_y = -(box.y_max + box.y_min) / 2.0;
    double offset_z = -(box.z_max + box.z_min) / 2.0;
    for (int i = 0; i < ase->n_meshes; i++) {
        mesh_t* mesh_curr = &ase->meshes[i];
        vertex_t* vertexes = mesh_curr->vertexes;
        for (int j = 0; j < mesh_curr->n_vertexes; j++) {
            vertex_t* vertex_curr = &vertexes[j];
            vertex_curr->x += offset_x;
            vertex_curr->y += offset_y;
            vertex_curr->z += offset_z;
        }
    }
    if (translate_x != NULL && translate_y != NULL && translate_z != NULL) {
        *translate_x = offset_x;
        *translate_y = offset_y;
        *translate_z = offset_z;
    }
}
