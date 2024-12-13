#include "Egg.h"

Egg::Egg(int density) {
    this->density = density;
    this->generateVerticies();
}
Egg::~Egg() {}

void Egg::calculateVertexPosition(GLfloat u, GLfloat v, Vertex& vertex) {
	GLfloat angle = numbers::pi * v;
    GLfloat xU, xV, yU, yV, zU, zV;


    vertex.position[0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(angle);
    vertex.position[1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2) - 5;
    vertex.position[2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(angle);

    vertex.color[0] = 1.0f;
    vertex.color[1] = 1.0f;
    vertex.color[2] = 1.0f;

	vertex.texCoord[0] = u;
	vertex.texCoord[1] = v;

    xU = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * cos(angle);
    xV = numbers::pi * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * sin(angle);
    yU = 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
    yV = 0.0;
    zU = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * sin(angle);
    zV = -numbers::pi * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(angle);


    GLfloat xVector = (yU * zV - zU * yV);
    GLfloat yVector = (zU * xV - xU * zV);
    GLfloat zVector = (xU * yV - yU * xV);
    GLfloat vectorLen = sqrt(pow(xVector, 2) + pow(yVector, 2) + pow(zVector, 2));

	if (vectorLen == 0) {
		vectorLen = 1;
	}

    vertex.normal[0] = xVector / vectorLen;
    vertex.normal[1] = yVector / vectorLen;
    vertex.normal[2] = zVector / vectorLen;

    // Flip normals for u >= 0.5
    if (u >= 0.5f) {
        vertex.normal[0] = -vertex.normal[0];
        vertex.normal[1] = -vertex.normal[1];
        vertex.normal[2] = -vertex.normal[2];
    }
}

// Method to generate vertices for the egg model
void Egg::generateVerticies() {
    vertices.clear();

    for (int du = 0; du < density; ++du) {
        GLfloat u = static_cast<GLfloat>(du) / (density - 1); 
        for (int dv = 0; dv < density; ++dv) {
            GLfloat v = static_cast<GLfloat>(dv) / (density - 1);
            Vertex vertex;
            calculateVertexPosition(u, v, vertex);
            vertices.push_back(vertex);
        }
    }
}

Egg& Egg::setDensity(int density) {
	int pom = density;
    if (pom % 2 == 0) {
        pom++;
    }
    this->density = pom;
    this->generateVerticies();
    return *this;
}
Egg& Egg::setPlainColors3f(GLfloat r, GLfloat g, GLfloat b) {

    for (Vertex& vertex : vertices) {
        vertex.color[0] = r;
        vertex.color[1] = g;
        vertex.color[2] = b;
    }

    return *this;
}
Egg& Egg::setRandomColors() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<GLfloat> dis(0.2f, 1.0f);

    for (Vertex& vertex : vertices) {
        vertex.color[0] = dis(gen);
        vertex.color[1] = dis(gen);
        vertex.color[2] = dis(gen);
    }

    return *this;
}

Egg& Egg::setMaterial()
{

    GLfloat diffuse_color[4];
    GLfloat specular_color[4];

    for (int i = 0; i < 3; ++i) {
        diffuse_color[i] = mat_color[i] * mat_diffuse;
        specular_color[i] = mat_color[i] * mat_specular;
    }
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    return *this;
}


void Egg::render_points() {
    glBegin(GL_POINTS); // Begin drawing points
    for (const Vertex& vertex : vertices) {
        glColor3f(vertex.color[0], vertex.color[1], vertex.color[2]);
        glVertex3f(vertex.position[0], vertex.position[1], vertex.position[2]);

    }
    glEnd();
}
void Egg::render_triangle() {

    int half = density / 2;
    const GLfloat* base_color = vertices[0].color;
    int bottom_row_start = density * (density - 1);
    int middle_row_start = density * half;

    // Set colors for specific rows
    for (int i = 0; i < density; ++i) {
        std::copy(base_color, base_color + 3, vertices[i].color);
        std::copy(base_color, base_color + 3, vertices[bottom_row_start + i].color);
        std::copy(base_color, base_color + 3, vertices[middle_row_start + i].color);
    }
    
    
    // Draw triangles
    for (int i = 0; i < half; i++) {
        glBegin(GL_TRIANGLE_STRIP);

        int row_start = density * i;
        int next_row_start = row_start + density;

        GLfloat ui = static_cast<GLfloat>(i) / (density - 1);

        // Draw triangle strip for the row
        for (int j = 0; j < density; j++) {
            glColor3fv(vertices[row_start + j].color);
            glTexCoord2fv(vertices[row_start + j].texCoord);
            glNormal3fv(vertices[row_start + j].normal);
            glVertex3fv(vertices[row_start + j].position);

            glColor3fv(vertices[next_row_start + j].color);
            glTexCoord2fv(vertices[next_row_start + j].texCoord);
            glNormal3fv(vertices[next_row_start + j].normal);
            glVertex3fv(vertices[next_row_start + j].position);
        }

        // Handle connections to the next row
        for (int j = 0; j < density - 1; j++) {
            int level = density * (density - 2) + j - i * density + 1;

            if (j == density - 2) {
                // Close the loop
                glColor3fv(vertices[row_start].color);
                glTexCoord2fv(vertices[row_start].texCoord);
                glNormal3fv(vertices[row_start].normal);
                glVertex3fv(vertices[row_start].position);

                glColor3fv(vertices[next_row_start].color);
                glTexCoord2fv(vertices[next_row_start].texCoord);
                glNormal3fv(vertices[next_row_start].normal);
                glVertex3fv(vertices[next_row_start].position);
            }
            else {
                // Add the next points in the strip
                glColor3fv(vertices[level + density].color);
                glTexCoord2fv(vertices[level + density].texCoord);
                glNormal3fv(vertices[level + density].normal);
                glVertex3fv(vertices[level + density].position);

                glColor3fv(vertices[level].color);
                glTexCoord2fv(vertices[level].texCoord);
                glNormal3fv(vertices[level].normal);
                glVertex3fv(vertices[level].position);
            }
        }

        glEnd();
    }
}