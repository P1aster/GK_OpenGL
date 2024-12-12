#include "Light.h"
#include <numbers>

Light::Light(GLenum lightId) {
    this->lightId = lightId;
}

Light::~Light() {}

Light& Light::setLightOptions() {
    GLfloat diffuse_color[4];
    GLfloat specular_color[4];

    for (int i = 0; i < 3; ++i) {
        diffuse_color[i] = light_color[i] * light_diffuse;
        specular_color[i] = light_color[i] * light_specular;
    }

    // Set the position of the light source in the 3D scene.
    // This vector determines the location and direction of the light in world space.
    // The position can also define directional or positional lighting depending on the `w` component (0 for directional light, 1 for positional light).
    glLightfv(lightId, GL_POSITION, light_position);

    // Configure the ambient light component for the light source.
    // Ambient light simulates indirect lighting that is scattered in all directions and uniformly illuminates all objects in the scene.
    // This setting defines the intensity and color of the ambient light.
    glLightfv(lightId, GL_AMBIENT, light_color);

    // Configure the diffuse light component for the light source.
    // Diffuse light is the direct light that is scattered equally in all directions, creating soft lighting effects.
    // This setting determines the light's color and intensity as it interacts with the surface of objects.
    glLightfv(lightId, GL_DIFFUSE, diffuse_color);

    // Configure the specular light component for the light source.
    // Specular light represents the reflection of light in a single direction, creating shiny highlights on surfaces.
    // This setting controls the intensity and color of the specular reflection.
    glLightfv(lightId, GL_SPECULAR, specular_color);

    // Set the constant attenuation factor for the light source.
    // Constant attenuation ensures the light intensity is unaffected by the distance from the light source.
    // This is useful for creating a consistent illumination level in the scene.
    glLightf(lightId, GL_CONSTANT_ATTENUATION, att_constant);

    // Set the linear attenuation factor for the light source.
    // Linear attenuation decreases the light intensity proportionally to the distance from the light source.
    // This creates a more natural fading effect as objects move further from the light.
    glLightf(lightId, GL_LINEAR_ATTENUATION, att_linear);

    // Set the quadratic attenuation factor for the light source.
    // Quadratic attenuation reduces the light intensity quadratically with distance, mimicking real-world light falloff.
    // This setting produces realistic lighting effects where brightness diminishes rapidly with distance.
    glLightf(lightId, GL_QUADRATIC_ATTENUATION, att_quadratic);

    // Define the direction in which the spotlight is pointing.
    // This vector specifies the spotlight's target and determines the light's directional effect within its cone.
    glLightfv(lightId, GL_SPOT_DIRECTION, spot_direction);

    // Configure the spotlight's cutoff angle in degrees.
    // The cutoff defines the angle of the spotlight's cone, beyond which the light has no effect.
    // A smaller cutoff angle results in a narrower beam of light.
    glLightf(lightId, GL_SPOT_CUTOFF, spot_cutoff);

    // Set the spotlight's exponent, which controls the intensity distribution within the cone.
    // A higher exponent results in a more focused light beam, while a lower exponent produces a softer spread.
    glLightf(lightId, GL_SPOT_EXPONENT, spot_exponent);

    return *this;
}

Light& Light::setLightPosition() {
    glLightfv(lightId, GL_SPOT_DIRECTION, spot_direction);
    glLightfv(lightId, GL_POSITION, light_position);

    return *this;
};

void Light::enableLight() {
    glEnable(lightId);
}

void Light::renderCone() {
    glPushMatrix();
    // Translate to the light position
    glTranslatef(light_position[0], light_position[1], light_position[2]);

    // Calculate the rotation angles to align the cone with the light direction
    GLfloat direction[3] = { spot_direction[0], spot_direction[1], spot_direction[2] };
    GLfloat length = sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);
    if (length != 0.0f) {
        direction[0] /= length;
        direction[1] /= length;
        direction[2] /= length;
    }

    GLfloat angle = acos(direction[2]) * 180.0 / std::numbers::pi;
    GLfloat axis[3] = { -direction[1], direction[0], 0.0f };

    // Rotate the cone to align with the light direction
    glRotatef(angle, axis[0], axis[1], axis[2]);

    // Rotate the cone 180 degrees to point in the correct direction
    glRotatef(180.0, 1.0, 0.0, 0.0);

    // Set the color of the cone
    glColor3f(light_color[0], light_color[1], light_color[2]);

    // Draw the larger wireframe cone
    glutWireCone(0.8, 1.6, 20, 2);

    glPopMatrix();
}

Light& Light::updateLightSpherePosition(GLdouble theta, GLdouble phi, GLdouble R) {
    this->R = R;
    this->theta = theta;
    this->phi = phi;

    theta = fmod(theta, 360.0);
    phi = fmod(phi, 360.0);

    GLdouble theta_rad = theta * (numbers::pi / 180.0);
    GLdouble phi_rad = phi * (numbers::pi / 180.0);

    theta_rad = fmod(theta_rad, 2.0 * numbers::pi);
    if (theta_rad < 0) theta_rad += 2.0 * numbers::pi;

    // Keep phi in [0, 2pi) range using modulo
    phi_rad = fmod(phi_rad, 2.0 * numbers::pi);
    if (phi_rad < 0) phi_rad += 2.0 * numbers::pi;

    light_position[0] = R * sin(theta_rad) * cos(phi_rad);
    light_position[1] = R * sin(phi_rad);
    light_position[2] = R * cos(theta_rad) * cos(phi_rad);

    // Calculate direction vector towards the origin
    GLfloat direction[3];
    direction[0] = -light_position[0];
    direction[1] = -light_position[1];
    direction[2] = -light_position[2];

    // Normalize the direction vector
        GLfloat length = sqrt(direction[0] * direction[0] +
            direction[1] * direction[1] +
            direction[2] * direction[2]);
    if (length != 0.0f) {
        direction[0] /= length;
        direction[1] /= length;
        direction[2] /= length;
    }

    // Update spot direction
    spot_direction[0] = direction[0];
    spot_direction[1] = direction[1];
    spot_direction[2] = direction[2];

    return *this;
}



Light& Light::setSpotDirection(GLfloat x, GLfloat y, GLfloat z) {
    spot_direction[0] = x;
    spot_direction[1] = y;
    spot_direction[2] = z;
    return *this;
}

Light& Light::setSpotCutoff(GLfloat cutoff) {
    spot_cutoff = cutoff;
    return *this;
}

Light& Light::setSpotExponent(GLfloat exponent) {
    spot_exponent = exponent;
    return *this;
}


Light& Light::setDiffuse(GLfloat diffuse) {
	light_diffuse = diffuse;
	return *this;
}

Light& Light::setSpecular(GLfloat specular) {
	light_specular = specular;
	return *this;
}

Light& Light::setAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic) {
	att_constant = constant;
	att_linear = linear;
	att_quadratic = quadratic;
	return *this;
}

Light& Light::setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	light_color[0] = red;
	light_color[1] = green;
	light_color[2] = blue;
	light_color[3] = alpha;
	return *this;
}

GLdouble Light::getPhi() {
    return phi;
}

GLdouble Light::getTheta() {
    return theta;
}

GLdouble Light::getR() {
    return R;
}
