#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
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
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����ģʽ��ζ������ֻ��ʹ��OpenGL���ܵ�һ���Ӽ�
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

    //vertex shader������ɫ��
    //������ɫ��Դ��
    const char* vertexShaderSource = "#version 330 core\n"//ʹ��in�ؼ��֣��ڶ�����ɫ�����������е����붥������(Input Vertex Attribute)��
        "layout (location = 0) in vec3 aPos;\n"//layout (location = 0)�趨�����������λ��ֵ(Location)
        "layout (location = 1) in vec3 aColor;"// ��ɫ����������λ��ֵΪ 1
        "layout (location = 2) in vec2 aTexCoord;"//�������������λ��ֵ2
        "uniform vec4 movePos;"
        "out vec3 ourColor;"// ��Ƭ����ɫ�����һ����ɫ
        "out vec2 TexCoord;"
        //"out vec4 vertexColor;"// ΪƬ����ɫ��ָ��һ����ɫ���
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos,1.0);\n"//gl_Position���õ�ֵ���Ϊ�ö�����ɫ���������
        "   ourColor = aColor;\n"// ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
        "   TexCoord=vec2(aTexCoord.x,aTexCoord.y);\n"
        //"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // �������������Ϊ����ɫ
        "}\0";
    //������ɫ��
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//glCreateShader������ɫ������,����ΪGL_VERTEX_SHADER
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//�������ɫ��Դ�븽�ӵ���ɫ��������
    glCompileShader(vertexShader);//������ɫ��
    //�ж��Ƿ����ɹ�
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //FragmentShaderƬ����ɫ��
    const char* fragmentShaderSource = "#version 330 core\n"//ʹ��in�ؼ��֣��ڶ�����ɫ�����������е����붥������(Input Vertex Attribute)��
        "out vec4 FragColor;\n"//layout (location = 0)�趨�����������λ��ֵ(Location)
        //"in vec4 vertexColor;"// �Ӷ�����ɫ�����������������������ͬ��������ͬ��
        //"uniform vec4 ourColor;" // ��OpenGL����������趨�������
        "in vec3 ourColor;"
        "in vec2 TexCoord;"
        "uniform sampler2D ourTexture;"
        "void main()\n"
        "{\n"
        " FragColor = texture(ourTexture, TexCoord)*vec4(ourColor,1.0f);"
        //"   FragColor =ourColor;\n//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"//gl_Position���õ�ֵ���Ϊ�ö�����ɫ���������
        "}\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //�ж��Ƿ����ɹ�
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();//������ɫ���������
    //��֮ǰ�������ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram��������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //�ж��Ƿ����ӳɹ�
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);//����������
    glDeleteShader(vertexShader);//����ɫ���������ӵ���������Ժ�
    glDeleteShader(fragmentShader);//�ǵ�ɾ����ɫ������

    //��������
    //����������
    float vertices[] = {
    //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // ����
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
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������
    int width, height, nrChannels;
    unsigned char* data = stbi_load("cat.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


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
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
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
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
