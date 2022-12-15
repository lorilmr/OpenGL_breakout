#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "shader_s.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

//��Դλ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2, lastY = screenHeight / 2;
float mixValue = 0.2f;
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��
bool firstMouse = true;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//����û��Ƿ����˷��ؼ�(Esc)
        //ͨ��glfwSetwindowShouldCloseʹ�ð�WindowShouldClose��������Ϊ true�ķ����ر�GLFW��
        //��һ��whileѭ����������⽫��ʧ�ܣ����򽫻�رա�
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
//�����Ļص����������Ǽ��㵱ǰ֡����һ֡���λ�õ�ƫ����
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);//�����Ƹ�����
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);//������fov��zoom���Ŵ���С
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����ģʽ��ζ������ֻ��ʹ��OpenGL���ܵ�һ���Ӽ�
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glViewport(0, 0, 800, 600);
    //GLFW��һ��ר�����OpenGL��C���Կ⣬���ṩ��һЩ��Ⱦ�������������޶ȵĽӿڡ�
    //GLFW�����û�����OpenGL�����ģ����崰�ڲ����Լ������û����롣
    //OpenGL��ɫ������(OpenGL Shading Language, GLSL)
    //GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //���ڴ�С�����ı��ʱ��,�����ӿڴ�С
    glfwSetCursorPosCallback(window, mouse_callback);//��GLFWע���˻ص�����֮�����һ�ƶ�mouse_callback�����ͻᱻ����
    glfwSetScrollCallback(window, scroll_callback);//ע�������ֵĻص�����

    //���������
    Shader lightingShader("shaders\\shader5.0_material.vs", "shaders\\shader5.0_material.fs");
    Shader lightCubeShader("shaders\\shader5.0_light.vs", "shaders\\shader5.0_light.fs");
    //��������
    //����������
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    //glm::vec3 cubePositions[] = {
    //glm::vec3(0.0f,  0.0f,  0.0f),
    //glm::vec3(2.0f,  5.0f, -15.0f),
    //glm::vec3(-1.5f, -2.2f, -2.5f),
    //glm::vec3(-3.8f, -2.0f, -12.3f),
    //glm::vec3(2.4f, -0.4f, -3.5f),
    //glm::vec3(-1.7f,  3.0f, -7.5f),
    //glm::vec3(1.3f, -2.0f, -2.5f),
    //glm::vec3(1.5f,  2.0f, -2.5f),
    //glm::vec3(1.5f,  0.2f, -1.5f),
    //glm::vec3(-1.3f,  1.0f, -1.5f)
    //};
    //unsigned int VBO;//����id,���ڵ��ö��㻺�����(Vertex Buffer Objects, VBO)
    //glGenBuffers(1, &VBO);//����һ��VBO����,
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);//���´����Ļ���󶨵�GL_ARRAY_BUFFER(���㻺�����Ļ�������)Ŀ����
    //����glBufferData�����������֮ǰ����Ķ�������vertices���Ƶ�������ڴ�GL_ARRAY_BUFFER��
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //�����������
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //���Ӷ�������
    //unsigned int VBO,VAO,EBO;
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 1. ��VAO
    glBindVertexArray(VAO);
    // 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. ���ö�������ָ��
    //ʹ��glVertexAttribPointer��������OpenGL����ν�����������
    //λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //���㷨����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    ////��ɫ����
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//����ÿ��������˵��λ�ö���������ǰ����������ƫ������0��
    //glEnableVertexAttribArray(1);//��ɫ���Խ���λ������֮������ƫ��������3 * sizeof(float)

    //Ϊ�ƹⴴ��һ���µ�VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ���õ�������Ķ������ԣ������ǵĵ���˵����ֻ��λ�����ݣ�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //���
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//���߿�ģʽ�������������
    glEnable(GL_DEPTH_TEST);//��Ȳ���

    while (!glfwWindowShouldClose(window))//Render Loop
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //���� �Ƿ���Esc
        processInput(window);


        //��Ⱦָ��
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//���Զ������ɫ�����Ļ��
        //glClear(GL_COLOR_BUFFER_BIT);//����һ������λ(Buffer Bit)��ָ��Ҫ��յĻ���
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)
        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

        lightingShader.use();

        //���ù�Դ
        lightingShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f); // note that all light colors are set at full intensity
        lightingShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        //glm::vec3 lightColor;
        //lightColor.x = sin(glfwGetTime() * 2.0f);
        //lightColor.y = sin(glfwGetTime() * 0.7f);
        //lightColor.z = sin(glfwGetTime() * 1.3f);
        //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
        //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        ////lightingShader.setVec3("light.position", lightPos);
        //lightingShader.setVec3("light.ambient", ambientColor);
        //lightingShader.setVec3("light.diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
        //lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        //���ò���
        //��ɫ����
        lightingShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
        lightingShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
        lightingShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
        lightingShader.setFloat("material.shininess", 32.0f);
        //lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        //lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        //lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        //lightingShader.setFloat("material.shininess", 32.0f);

        //lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        //lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);//���ù�Դλ��==�������䣨���ܵ��Ĺ�Ķ��٣�
        lightingShader.setVec3("viewPos", camera.Position);//����������۾���λ��==��ֱ�ӹ���
        //����任
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();//glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        lightingShader.setMat4("view", view);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);//radians(fov)
        lightingShader.setMat4("projection", projection);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //trans = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f)); // λ��
        //float scaleAmount = sin(glfwGetTime());
        //scaleAmount = scaleAmount > 0 ? scaleAmount : -scaleAmount;//����ת
        //trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
        /*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
        //glUseProgram(shaderProgram); //��������Ⱦһ������ʱҪʹ����ɫ������
        //glBindVertexArray(VAO);//ֻҪ�ڻ�������ǰ�򵥵ذ�VAO�󶨵�ϣ��ʹ�õ��趨�Ͼ�����

        //���ƹ�Դ
        lightCubeShader.use();
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setMat4("projection", projection);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model",model);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        //glDrawArrays(GL_TRIANGLES, 0, 36);//glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//6������

        //�������رմ���֮ǰ���ϻ���ͼ���ܹ������û�����
        glfwSwapBuffers(window);
        //���������û�д���ʲô�¼�������������롢����ƶ��ȣ�
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
