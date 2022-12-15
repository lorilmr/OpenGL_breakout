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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//检查用户是否按下了返回键(Esc)
        //通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为 true的方法关闭GLFW。
        //下一次while循环的条件检测将会失败，程序将会关闭。
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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式意味着我们只能使用OpenGL功能的一个子集
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
    Shader ourShader("shaders\\shader.vs","shaders\\shader.fs");

    //顶点输入
    //三角形坐标
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上 分别指定为红色、绿色、蓝色和黄色
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,    // 左上
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
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //unsigned int texture;
    //glGenTextures(1, &texture);
    //glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    //设置纹理环绕参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 加载并生成纹理
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);//告诉stb_image.h在y轴上翻转加载的纹理
    //stb_image.h将会用图像的宽度、高度和颜色通道的个数填充这三个变量
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
    // 为当前绑定的纹理对象设置环绕、过滤方式
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
        //请注意，awesomeface.png具有透明度，因此是一个alpha通道，所以请确保告诉OpenGL数据类型为GL_RGBA。
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    ourShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
    ourShader.setInt("texture2", 1); // 或者使用着色器类设置

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
       
        //矩阵变换
        //glm::mat4 trans = glm::mat4(1.0f);//单位矩阵
        //把箱子在每个轴都缩放到0.5倍，然后沿z轴旋转90度。
        //用glm::radians将角度转化为弧度。
        //注意有纹理的那面矩形是在XY平面上的，所以我们需要把它绕着z轴旋转。
        
        //trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f)); // 位移
        //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));//随时间旋转
       
        //缩放
        //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));


        //unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
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
        //glBindTexture(GL_TEXTURE_2D, texture);
        //在相应的纹理单元上绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ourShader.use();
        ourShader.setFloat("mixValue", mixValue);
        //坐标变换
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", model);
        glm::mat4 view = glm::mat4(1.0f);
        // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        ourShader.setMat4("view", view);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        
        //trans = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f)); // 位移
        //float scaleAmount = sin(glfwGetTime());
        //scaleAmount = scaleAmount > 0 ? scaleAmount : -scaleAmount;//不翻转
        //trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
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

    glfwTerminate();
    return 0;
}
