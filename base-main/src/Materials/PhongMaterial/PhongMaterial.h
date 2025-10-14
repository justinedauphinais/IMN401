

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

        l_Model = glGetUniformLocation(vp->getId(), "Model");
        l_View = glGetUniformLocation(vp->getId(), "View");
        l_Proj = glGetUniformLocation(vp->getId(), "Proj");

        l_objectColor = glGetUniformLocation(fp->getId(), "objectColor");
        l_ka = glGetUniformLocation(fp->getId(), "ka");
        l_kd = glGetUniformLocation(fp->getId(), "kd");
        l_ks = glGetUniformLocation(fp->getId(), "ks");
        l_s = glGetUniformLocation(fp->getId(), "s");
        l_posLum = glGetUniformLocation(vp->getId(), "posLum");
        l_posCam= glGetUniformLocation(vp->getId(), "posCam");
    }

    ~PhongMaterial();

    virtual void render(Node *o) {

        m_ProgramPipeline->bind();

        o->drawGeometry(GL_TRIANGLES);
        m_ProgramPipeline->release();
    };

    virtual void animate(Node *o, const float elapsedTime) {
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
        Node *Lumiere = scene->getNode("Lumiere");
        glm::vec3 P = glm::vec3(0.0f, 5.0f, 0.0f);
        glm::vec3 P_objet = o->frame()->convertPtFrom(P, Lumiere->frame());

        Camera *camera = Scene::getInstance()->camera();
        glm::vec3 pCam = glm::vec3(0.0f, 0.0f, 0.0f);
        pCam = camera->frame()->convertPtTo(pCam, o->frame());
       
        if (l_posLum >= 0) glProgramUniform3fv(progID, l_posLum, 1, glm::value_ptr(P_objet));
        if (l_posCam >= 0) glProgramUniform3fv(progID, l_posLum, 1, glm::value_ptr(P_objet));
        if (l_objectColor >= 0) glProgramUniform3fv(fp->getId(), l_objectColor, 1, glm::value_ptr(m_objectColor));

        if (l_ka >= 0) glProgramUniform1f(fp->getId(), l_kd, kd);
        if (l_kd >= 0) glProgramUniform1f(fp->getId(), l_ka, ka);
        if (l_ks >= 0) glProgramUniform1f(fp->getId(), l_ks, ks);
        if (l_s >= 0) glProgramUniform1f(fp->getId(), l_s, s);
    }

    virtual void displayInterface() {};

protected:
    GLProgram *vp;
    GLProgram *fp;

    GLuint l_View, l_Proj, l_Model; // location of uniforms

    GLint l_Time = -1, l_Amp = -1, l_Freq = -1, l_posLum, l_kd, l_ks, l_ka, l_s, l_posCam, l_lightColor, l_objectColor;

    float m_time = 0.0f;
    float m_amp = 0.001f;
    float m_freq = 0.5f;

    glm::vec3 m_objectColor = glm::vec3(1.0f, 0.0f, 0.0f);

    float posLum, kd = 0.8f, ks = 0.5f, ka = 0.2f, s = 16.0f;
};

#endif