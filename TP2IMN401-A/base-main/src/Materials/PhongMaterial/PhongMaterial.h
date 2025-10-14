

#ifndef _PhongMaterial_
#define _PhongMaterial_

#include "MaterialGL.h"
#include <glm/gtc/type_ptr.hpp>

class PhongMaterial : public MaterialGL {
public:
    PhongMaterial(std::string name = "") {

        vp = new GLProgram(MaterialPath + "PhongMaterial/Phong-VS.glsl", GL_VERTEX_SHADER);
        fp = new GLProgram(MaterialPath + "PhongMaterial/Phong-FS.glsl", GL_FRAGMENT_SHADER);

        m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
        m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

        l_View = glGetUniformLocation(vp->getId(), "View");
        l_Proj = glGetUniformLocation(vp->getId(), "Proj");
        l_Model = glGetUniformLocation(vp->getId(), "Model");

        l_Time = glGetUniformLocation(vp->getId(), "u_time");
        l_Amp = glGetUniformLocation(vp->getId(), "u_amp");
        l_Freq = glGetUniformLocation(vp->getId(), "u_freq");

        l_ks = glGetUniformLocation(fp->getId(), "ks");
        l_kd = glGetUniformLocation(fp->getId(), "kd");
        l_ka = glGetUniformLocation(fp->getId(), "ka");
        l_s = glGetUniformLocation(fp->getId(), "s");

        l_posCam = glGetUniformLocation(fp->getId(), "posCam");
        l_posLum = glGetUniformLocation(vp->getId(), "posLum");
    }

    ~PhongMaterial();

    virtual void render(Node *o) {

        m_ProgramPipeline->bind();

        o->drawGeometry(GL_TRIANGLES);
        m_ProgramPipeline->release();
    };

    virtual void animate(Node *o, const float elapsedTime) {

        /**********************************************
        TP 2 - A completer
        Calculer et Transmettre les matrices Model View et Proj au shaders
        - Utilisez glm::value_ptr(mat) pour trouver le pointeur de la matrice mat a transmettre au GPU via la fonction glProgramUniform*()
        - Une matrice 4X4 se transmet grace a glProgramUniformMatrix4fv
        ***********************************************/

        glm::mat4 Model = o->frame()->getModelMatrix();
        glm::mat4 View = scene->camera()->getViewMatrix();
        glm::mat4 Proj = scene->camera()->getProjectionMatrix();

        const GLuint progID = vp->getId();
        if (l_Model >= 0) glProgramUniformMatrix4fv(progID, l_Model, 1, GL_FALSE, glm::value_ptr(Model));
        if (l_View >= 0) glProgramUniformMatrix4fv(progID, l_View, 1, GL_FALSE, glm::value_ptr(View));
        if (l_Proj >= 0) glProgramUniformMatrix4fv(progID, l_Proj, 1, GL_FALSE, glm::value_ptr(Proj));

        // Deformation du bunny
        if (elapsedTime < 20) m_time += elapsedTime / 1000;

        if (l_Time >= 0) glProgramUniform1f(progID, l_Time, m_time);
        if (l_Amp >= 0) glProgramUniform1f(progID, l_Amp, m_amp);
        if (l_Freq >= 0) glProgramUniform1f(progID, l_Freq, m_freq);

        // Phong
        Camera *camera = Scene::getInstance()->camera();
        glm::vec3 posCam = (camera->frame()->convertPtTo(glm::vec3(0.0, 0.0, 0.0), o->frame()));
        glProgramUniform3f(progID, l_posLum, 0.0f, 5.0f, 0.0f);
        glProgramUniform3f(progID, l_posCam, posCam.x, posCam.y, posCam.z);

        glProgramUniform1f(fp->getId(), l_kd, kd);
        glProgramUniform1f(fp->getId(), l_ks, ks);
        glProgramUniform1f(fp->getId(), l_ka, ka);
        glProgramUniform1f(fp->getId(), l_s, s);
    }

    virtual void displayInterface() {};

protected:
    GLProgram *vp;
    GLProgram *fp;

    GLuint l_View, l_Proj, l_Model; // location of uniforms

    GLint l_Time = -1, l_Amp = -1, l_Freq = -1, l_posLum, l_kd, l_ks, l_ka, l_s, l_posCam;

    float m_time = 0.0f;
    float m_amp = 0.001f;
    float m_freq = 0.5f;

    float posLum, kd = 1.0f, ks = 1.0f, ka = 1.0f, s = 16.0f;
};

#endif