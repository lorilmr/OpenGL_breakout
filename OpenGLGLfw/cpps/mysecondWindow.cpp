#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float mixValue = 0.2f;

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
}
unsigned int screenWidth = 800;
unsigned int screenHeight = 600;

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

    //���������
    Shader ourShader("shaders\\shader.vs","shaders\\shader.fs");

    //��������
    //����������
    float vertices[] = {
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ���� �ֱ�ָ��Ϊ��ɫ����ɫ����ɫ�ͻ�ɫ
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // ����
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,    // ����
        //// positions         // colors
        //0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        //-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
        //0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
        //��ʼ������
        //-0.5f, -0.5f, 0.0f,
        // 0.5f, -0.5f, 0.0f,
        // 0.0f,  0.5f, 0.0f,
        // ����
        //0.5f,  0.5f, 0.0f,  // top right
        //0.5f, -0.5f, 0.0f,  // bottom right
        //-0.5f, -0.5f, 0.0f,  // bottom left
        //-0.5f,  0.5f, 0.0f   // top left 
        //// first triangle
        //-0.9f, -0.5f, 0.0f,  // left 
        //-0.0f, -0.5f, 0.0f,  // right
        //-0.45f, 0.5f, 0.0f,  // top 
        //// second triangle
        //0.0f, -0.5f, 0.0f,  // left
        //0.9f, -0.5f, 0.0f,  // right
        //0.45f, 0.5f, 0.0f   // top 
    };
    //������
    unsigned int indices[] = { // ע��������0��ʼ! 
    0, 1, 3, // ��һ��������
    1, 2, 3  // �ڶ���������
    };
    //unsigned int VBO;//����id,���ڵ��ö��㻺�����(Vertex Buffer Objects, VBO)
    //glGenBuffers(1, &VBO);//����һ��VBO����,
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);//���´����Ļ���󶨵�GL_ARRAY_BUFFER(���㻺�����Ļ�������)Ŀ����
    //����glBufferData�����������֮ǰ����Ķ�������vertices���Ƶ�������ڴ�GL_ARRAY_BUFFER��
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //����
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //unsigned int texture;
    //glGenTextures(1, &texture);
    //glBindTexture(GL_TEXTURE_2D, texture);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    //���������Ʋ���
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //����������˲���
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // ���ز���������
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);//����stb_image.h��y���Ϸ�ת���ص�����
    //stb_image.h������ͼ��Ŀ�ȡ��߶Ⱥ���ɫͨ���ĸ����������������
    unsigned char* data = stbi_load("pictures\\cat.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("pictures\\awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        //��ע�⣬awesomeface.png����͸���ȣ������һ��alphaͨ����������ȷ������OpenGL��������ΪGL_RGBA��
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    ourShader.use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // �ֶ�����
    ourShader.setInt("texture2", 1); // ����ʹ����ɫ��������

    //�����������
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //���Ӷ�������
    //unsigned int VBO,VAO,EBO;
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 1. ��VAO
    glBindVertexArray(VAO);
    // 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. ���ö�������ָ��
    //ʹ��glVertexAttribPointer��������OpenGL����ν�����������
    //λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//����ÿ��������˵��λ�ö���������ǰ����������ƫ������0��
    glEnableVertexAttribArray(1);//��ɫ���Խ���λ������֮������ƫ��������3 * sizeof(float)
    //��������
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //���
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//���߿�ģʽ�������������

    while (!glfwWindowShouldClose(window))//Render Loop
    {
        //���� �Ƿ���Esc
        processInput(window);

        //��Ⱦָ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���Զ������ɫ�����Ļ��
        glClear(GL_COLOR_BUFFER_BIT);//����һ������λ(Buffer Bit)��ָ��Ҫ��յĻ���
       
        //����任
        //glm::mat4 trans = glm::mat4(1.0f);//��λ����
        //��������ÿ���ᶼ���ŵ�0.5����Ȼ����z����ת90�ȡ�
        //��glm::radians���Ƕ�ת��Ϊ���ȡ�
        //ע��������������������XYƽ���ϵģ�����������Ҫ��������z����ת��
        
        //trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f)); // λ��
        //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));//��ʱ����ת
       
        //����
        //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));


        //unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        //float timeValue = glfwGetTime();//��ȡ���е�����
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//��ɫ��ʱ��仯
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");//��glGetUniformLocation��ѯuniform ourColor��λ��ֵ
        //glUseProgram(shaderProgram);//���������� ����һ��uniform֮ǰ�������ʹ�ó���(����glUseProgram)
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);//vec4 outColor
        //glUseProgram(shaderProgram);
        //glUniform4f(glGetUniformLocation(shaderProgram, "movePos"), 1.0f, 0.0f, 0.0f, 1.0f);

        //����һ��������
        // 4. ��������
        // ����
        //glBindTexture(GL_TEXTURE_2D, texture);
        //����Ӧ������Ԫ�ϰ�����
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ourShader.use();
        ourShader.setFloat("mixValue", mixValue);
        //����任
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", model);
        glm::mat4 view = glm::mat4(1.0f);
        // ע�⣬���ǽ�����������Ҫ�����ƶ������ķ������ƶ���
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        ourShader.setMat4("view", view);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        
        //trans = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f)); // λ��
        //float scaleAmount = sin(glfwGetTime());
        //scaleAmount = scaleAmount > 0 ? scaleAmount : -scaleAmount;//����ת
        //trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glUseProgram(shaderProgram); //��������Ⱦһ������ʱҪʹ����ɫ������
        //glBindVertexArray(VAO);//ֻҪ�ڻ�������ǰ�򵥵ذ�VAO�󶨵�ϣ��ʹ�õ��趨�Ͼ�����
        //glDrawArrays(GL_TRIANGLES, 0, 3);//glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//6������

        //�������رմ���֮ǰ���ϻ���ͼ���ܹ������û�����
        glfwSwapBuffers(window);
        //���������û�д���ʲô�¼�������������롢����ƶ��ȣ�
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
