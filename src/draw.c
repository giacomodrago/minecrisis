#include <string.h>
#include "texture.h"
#include "draw.h"


void drawSphere(double radius, int sphere_detail, bool textured) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    if (textured) gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, radius, sphere_detail, sphere_detail);
    gluDeleteQuadric(quadric);
}

void drawAxes(double x_len, double y_len, double z_len) {
	glPushMatrix();
        glScalef(x_len, y_len, z_len);
        glLineWidth(3.0);
        glBegin(GL_LINES);
            // x red
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);
            // y green
            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 1.0, 0.0);
            // z blue
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, 1.0);
        glEnd();
	glPopMatrix();
}

void drawWorldBox(double width, double height, double depth) {
    glPushMatrix();
        glScaled(width, height, depth);
        glLineWidth(2.0);
        glColor3f(1.0, 1.0, 1.0);
        glutWireCube(1.0);
    glPopMatrix();
}

void drawSegment(point_t p1, point_t p2) {
    glLineWidth(2.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);
    glEnd();
}

void drawStarmap(texture_t texture_id) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    glColor4fv(starmap_color);
    glBindTexture(GL_TEXTURE_2D, texture_id); // bind texture
    drawSphere(1.0, STARMAP_DETAIL, true);
    glBindTexture(GL_TEXTURE_2D, -1); // unbind textures
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
}

void drawPlanet(texture_t texture_id) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glColor4fv(planet_color);
    glBindTexture(GL_TEXTURE_2D, texture_id); // bind texture
    drawSphere(PLANET_RADIUS, PLANET_DETAIL, true);
    glBindTexture(GL_TEXTURE_2D, -1); // unbind texture
}

void drawRotatedPlanet(display_list_t display_list, double planet_rot_x, double planet_rot_y) {
    glPushMatrix();
        glTranslated(0.0, 0.0, PLANET_OFFSET);
        glRotated(planet_rot_x, 0.0, 1.0, 0.0);
        glRotated(planet_rot_y, -1.0, 0.0, 0.0);
        glRotated(90.0, 1.0, 0.0, 0.0);
        glCallList(display_list);
    glPopMatrix();
}

void drawCheckpointSphere(int detail) {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    int sphere_detail;
    if (detail == 0) { // low
        sphere_detail = CHECKPOINT_DETAIL_0;
    } else if (detail == 1) { // med
        sphere_detail = CHECKPOINT_DETAIL_1;
    } else { // high
        sphere_detail = CHECKPOINT_DETAIL_2;
    }
    glDepthMask(false);
    drawSphere(CHECKPOINT_RADIUS, sphere_detail, false);
    glDepthMask(true);

}

void drawCheckpoint(display_list_t display_list, double alpha) {
    const float checkpoint_color_alpha[] =
        {checkpoint_color[0],
        checkpoint_color[1],
        checkpoint_color[2],
        checkpoint_color[3] * alpha};
    // Set material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, checkpoint_color_alpha);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_color);
    glCallList(display_list);
}

void drawMine(int detail) {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mine_diffuse);
    glMateriali(GL_FRONT, GL_SHININESS, mine_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mine_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_color);

    glDepthMask(false);

    // Draw the object
    glPushMatrix();
        glScaled(MINE_RADIUS, MINE_RADIUS, MINE_RADIUS);
        int sphere_detail;
        if (detail == 0) sphere_detail = MINE_DETAIL_0; // low
        else if (detail == 1) sphere_detail = MINE_DETAIL_1; // med
        else if (detail == 2) sphere_detail = MINE_DETAIL_2; // high
        else sphere_detail = MINE_DETAIL_3; // highest
        drawSphere(1.0, sphere_detail, false);
    glPopMatrix();

    glDepthMask(true);

}

void drawTurret(int detail) {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, turret_color);
    glMateriali(GL_FRONT, GL_SHININESS, turret_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, turret_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_color);

    // Draw the object
    glPushMatrix();
        glScaled(TURRET_RADIUS, TURRET_RADIUS, TURRET_RADIUS);
        int sphere_detail = 100;
        if (detail == 0) sphere_detail = TURRET_DETAIL_0; // low
        else if (detail == 1) sphere_detail = TURRET_DETAIL_1; // med
        else if (detail == 2) sphere_detail = TURRET_DETAIL_2; // high
        else sphere_detail = TURRET_DETAIL_3; // highest
        drawSphere(STARMAP_RADIUS, sphere_detail, false);
    glPopMatrix();

}


void getShipTransformationMatrix(double transform_matrix[9], vector_t position,
                            vector_t vector_look, vector_t vector_up) {
    vector_t right_vector = cross_product(vector_look, vector_up);
    double matrix[] =
        {right_vector.x, right_vector.y, right_vector.z, 0.0,
        vector_up.x, vector_up.y, vector_up.z, 0.0,
        -vector_look.x, -vector_look.y, -vector_look.z, 0.0,
        position.x, position.y, position.z, 1.0};
    memcpy(transform_matrix, matrix, sizeof(matrix));
}

void drawShip(display_list_t ship_body,
              display_list_t ship_weapons,
              display_list_t ship_glass,
              display_list_t ship_decorations,
              display_list_t ship_glow,
              vector_t position,
              vector_t vector_look,
              vector_t vector_up,
              double inclination,
              double throttle,
              double opacity,
              bool draw_bounding_boxes,
              bounding_boxes_t* boxes) {

    float ship_body_color_curr[] = {
        ship_body_color[0],
        ship_body_color[1],
        ship_body_color[2],
        ship_body_color[3] * opacity
    };
    float ship_glass_color_curr[] = {
        ship_glass_color[0],
        ship_glass_color[1],
        ship_glass_color[2],
        ship_glass_color[3] * opacity
    };
    float ship_decorations_color_curr[] = {
        ship_decorations_color[0],
        ship_decorations_color[1],
        ship_decorations_color[2],
        ship_decorations_color[3] * opacity
    };
    float ship_glow_fake_color_curr[] = {
        0.0,
        0.0,
        0.0,
        ship_glow_color[3] * opacity
    };
    float ship_glow_color_curr[] = {
        ship_glow_color[0] * throttle,
        ship_glow_color[1] * throttle,
        ship_glow_color[2] * throttle,
        ship_glow_color[3] * opacity
    };

    double matrix[16];
    getShipTransformationMatrix(matrix, position, vector_look, vector_up);

    glDisable(GL_CULL_FACE);

    glPushMatrix();

        glMultMatrixd(matrix);
        glRotated(radToDeg(inclination), 0.0, 0.0, 1.0);

        glPushMatrix();

            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_color);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_color);

            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ship_body_color_curr);
            glCallList(ship_body);
            glCallList(ship_weapons);

            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ship_glass_color_curr);
            glCallList(ship_glass);

            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ship_decorations_color_curr);
            glCallList(ship_decorations);

            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ship_glow_fake_color_curr);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ship_glow_color_curr);
            glCallList(ship_glow);

            // Draw bounding boxes
            if (draw_bounding_boxes) {
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
                glLineWidth(1.0);
                for (int i = 0; i < boxes->n_boxes; i++) {
                    bounding_box_t* curr_box = &boxes->array[i];
                    double width = curr_box->x_max - curr_box->x_min;
                    double height = curr_box->y_max - curr_box->y_min;
                    double depth = curr_box->z_max - curr_box->z_min;
                    glPushMatrix();
                        glTranslated(curr_box->x_min + width / 2.0,
                                     curr_box->y_min + height / 2.0,
                                     curr_box->z_min + depth / 2.0);
                        glScaled(width, height, depth);
                        glutWireCube(1.0);
                    glPopMatrix();
                }
            }

        glPopMatrix();

    glPopMatrix();

}

void drawShipShield(vector_t position, vector_t vector_look, vector_t vector_up,
                double inclination, double intensity) {

    double matrix[16];
    getShipTransformationMatrix(matrix, position, vector_look, vector_up);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    double alpha = SHIELD_MAX_ALPHA * min(1.0, intensity);

    glDepthMask(false);

    float ship_shield_color_alpha[] =
        {ship_shield_color[0], ship_shield_color[1], ship_shield_color[2], alpha};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ship_shield_color_alpha);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_color);

    glPushMatrix();
        glMultMatrixd(matrix);
        glRotated(radToDeg(inclination), 0.0, 0.0, 1.0);
        glPushMatrix();
            glScaled(SHIP_RADIUS_X, SHIP_RADIUS_Y, SHIP_RADIUS_Z);
            drawSphere(1.0, SHIELD_DETAIL, false);
        glPopMatrix();
    glPopMatrix();

    glDepthMask(true);

}

void drawShipShieldPoints(point_t* points, int n_points,
                          vector_t position, vector_t vector_look, vector_t vector_up,
                          double inclination) {
    double matrix[16];
    getShipTransformationMatrix(matrix, position, vector_look, vector_up);
    glPointSize(1.0);
    glColor3f(1.0, 1.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
        glMultMatrixd(matrix);
        glRotated(radToDeg(inclination), 0.0, 0.0, 1.0);
        glBegin(GL_POINTS);
        for (int i = 0; i < n_points; i++) {
            glVertex3d(points[i].x, points[i].y, points[i].z);
        }
        glEnd();
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void drawShot() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glColor4fv(shot_color);
    glutSolidSphere(SHOT_RADIUS, SHOT_DETAIL, SHOT_DETAIL);
}

void drawExplosion(explosion_t* explosion, vector_t position, vector_t vector_look, vector_t vector_up) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    double matrix[16];
    getShipTransformationMatrix(matrix, position, vector_look, vector_up);
    glPushMatrix();
        glMultMatrixd(matrix);
        explosion_draw(explosion);
    glPopMatrix();
}

void draw2DWireRectangle(int left, int top, int width, int height, const float color[4], float line_width) {
    glColor4fv(color);
    glLineWidth(line_width);
    glBegin(GL_LINE_LOOP);
        glVertex2i(left, top+height);
        glVertex2i(left+width, top+height);
        glVertex2i(left+width, top);
        glVertex2i(left, top);
    glEnd();
}

void draw2DSolidRectangle(int left, int top, int width, int height, const float color[4]) {
    glColor4fv(color);
    glBegin(GL_POLYGON);
        glVertex2i(left, top+height);
        glVertex2i(left+width, top+height);
        glVertex2i(left+width, top);
        glVertex2i(left, top);
    glEnd();
}

void draw2DTexturedRectangle(int left, int top, int width, int height, const float color[4],
                           texture_t texture_id,
                           double tcoord_u_left, double tcoord_u_right,
                           double tcoord_v_bottom, double tcoord_v_top) {
    glColor4fv(color);
    glBindTexture(GL_TEXTURE_2D, texture_id); // bind texture
    glBegin(GL_POLYGON);
        glTexCoord2f(tcoord_u_left, tcoord_v_bottom);
        glVertex2i(left, top+height);
        glTexCoord2f(tcoord_u_right, tcoord_v_bottom);
        glVertex2i(left+width, top+height);
        glTexCoord2f(tcoord_u_right, tcoord_v_top);
        glVertex2i(left+width, top);
        glTexCoord2f(tcoord_u_left, tcoord_v_top);
        glVertex2i(left, top);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, -1); // unbind textures
}

inline void lookAt(vector_t position, vector_t vector_look, vector_t vector_up) {
    vector_t vector_look_translated = vectors_sum(position, vector_look);
    gluLookAt(position.x, position.y, position.z,
        vector_look_translated.x, vector_look_translated.y, vector_look_translated.z,
        vector_up.x, vector_up.y, vector_up.z);
}
