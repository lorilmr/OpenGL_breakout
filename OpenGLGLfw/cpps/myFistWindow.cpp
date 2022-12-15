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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//检查用户是否按下了返回键(Esc)
        //通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为 true的方法关闭GLFW。
        //下一次while循环的条件检测将会失败，程序将会关闭。
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式意味着我们只能使用OpenGL功能的一个子集
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
    //GLFW是一个专门针对OpenGL的C语言库，它提供了一些渲染物体所需的最低限度的接口。
    //GLFW允许用户创建OpenGL上下文，定义窗口参数以及处理用户输入。
    //OpenGL着色器语言(OpenGL Shading Language, GLSL)
    //GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //窗口大小发生改变的时候,调整视口大小

    //你好三角形

    //vertex shader顶点着色器
    //顶点着色器源码
    const char* vertexShaderSource = "#version 330 core\n"//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
        "layout (location = 0) in vec3 aPos;\n"//layout (location = 0)设定了输入变量的位置值(Location)
        "layout (location = 1) in vec3 aColor;"// 颜色变量的属性位置值为 1
        "layout (location = 2) in vec2 aTexCoord;"//纹理变量的属性位置值2
        "uniform vec4 movePos;"
        "out vec3 ourColor;"// 向片段着色器输出一个颜色
        "out vec2 TexCoord;"
        //"out vec4 vertexColor;"// 为片段着色器指定一个颜色输出
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos,1.0);\n"//gl_Position设置的值会成为该顶点着色器的输出。
        "   ourColor = aColor;\n"// 将ourColor设置为我们从顶点数据那里得到的输入颜色
        "   TexCoord=vec2(aTexCoord.x,aTexCoord.y);\n"
        //"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // 把输出变量设置为暗红色
        "}\0";
    //编译着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//glCreateShader创建着色器对象,类型为GL_VERTEX_SHADER
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//把这个着色器源码附加到着色器对象上
    glCompileShader(vertexShader);//编译着色器
    //判定是否编译成功
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //FragmentShader片段着色器
    const char* fragmentShaderSource = "#version 330 core\n"//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
        "out vec4 FragColor;\n"//layout (location = 0)设定了输入变量的位置值(Location)
        //"in vec4 vertexColor;"// 从顶点着色器传来的输入变量（名称相同、类型相同）
        //"uniform vec4 ourColor;" // 在OpenGL程序代码中设定这个变量
        "in vec3 ourColor;"
        "in vec2 TexCoord;"
        "uniform sampler2D ourTexture;"
        "void main()\n"
        "{\n"
        " FragColor = texture(ourTexture, TexCoord)*vec4(ourColor,1.0f);"
        //"   FragColor =ourColor;\n//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"//gl_Position设置的值会成为该顶点着色器的输出。
        "}\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //判断是否编译成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();//创建着色器程序对象
    //把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //判断是否连接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);//激活程序对象
    glDeleteShader(vertexShader);//把着色器对象链接到程序对象以后，
    glDeleteShader(fragmentShader);//记得删除着色器对象

    //顶点输入
    //三角形坐标
    float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 左上
    //// positions         // colors
    //0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    //0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    //初始三角形
    //-0.5f, -0.5f, 0.0f,
    // 0.5f, -0.5f, 0.0f,
    // 0.0f,  0.5f, 0.0f,
    // 矩形
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
    //边索引
    unsigned int indices[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
    };
    //unsigned int VBO;//缓冲id,用于调用顶点缓冲对象(Vertex Buffer Objects, VBO)
    //glGenBuffers(1, &VBO);//生成一个VBO对象,
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);//把新创建的缓冲绑定到GL_ARRAY_BUFFER(顶点缓冲对象的缓冲类型)目标上
    //调用glBufferData函数，它会把之前定义的顶点数据vertices复制到缓冲的内存GL_ARRAY_BUFFER中
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //纹理
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
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


    //索引缓冲对象
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //链接顶点属性
    //unsigned int VBO,VAO,EBO;
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    //使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据
    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//对于每个顶点来说，位置顶点属性在前，所以它的偏移量是0。
    glEnableVertexAttribArray(1);//颜色属性紧随位置数据之后，所以偏移量就是3 * sizeof(float)
    //纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //解绑
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//用线框模式绘制你的三角形

    while (!glfwWindowShouldClose(window))//Render Loop
    {
        //输入 是否按下Esc
        processInput(window);

        //渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//用自定义的颜色清空屏幕。
        glClear(GL_COLOR_BUFFER_BIT);//接受一个缓冲位(Buffer Bit)来指定要清空的缓冲

        //float timeValue = glfwGetTime();//获取运行的秒数
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//颜色随时间变化
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");//用glGetUniformLocation查询uniform ourColor的位置值
        //glUseProgram(shaderProgram);//激活程序对象 更新一个uniform之前你必须先使用程序(调用glUseProgram)
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);//vec4 outColor
        //glUseProgram(shaderProgram);
        //glUniform4f(glGetUniformLocation(shaderProgram, "movePos"), 1.0f, 0.0f, 0.0f, 1.0f);

        //画第一个三角形
        // 4. 绘制物体
        // 纹理
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glUseProgram(shaderProgram); //当我们渲染一个物体时要使用着色器程序
        //glBindVertexArray(VAO);//只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行了
        //glDrawArrays(GL_TRIANGLES, 0, 3);//glDrawArrays函数第一个参数是我们打算绘制的OpenGL图元的类型。
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//6个顶点

        //在主动关闭窗口之前不断绘制图像并能够接受用户输入
        glfwSwapBuffers(window);
        //函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）
        glfwPollEvents();
    }
   
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
