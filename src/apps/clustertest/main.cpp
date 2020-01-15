/*****************************************************************************************
 * SGCT                                                                                  *
 * Simple Graphics Cluster Toolkit                                                       *
 *                                                                                       *
 * Copyright (c) 2012-2020                                                               *
 * For conditions of distribution and use, see copyright notice in LICENSE.md            *
 ****************************************************************************************/

#include <sgct/sgct.h>

namespace {
    constexpr const int ExtendedSize = 10000;

    struct {
        GLuint vao;
        GLuint vbo;
        GLuint ibo;
    } geometry;
    int nVertices = 0;
    GLint matrixLocation = -1;

    sgct::SharedDouble currentTime(0.0);
    sgct::SharedWString sTimeOfDay;
    sgct::SharedBool extraPackages(false);
    sgct::SharedBool barrier(false);
    sgct::SharedBool stats(false);
    sgct::SharedBool takeScreenshot(false);
    sgct::SharedBool slowRendering(false);
    sgct::SharedBool frametest(false);
    sgct::SharedFloat speed(5.f);
    sgct::SharedVector<float> extraData;

    constexpr const char* vertexShader = R"(
#version 330 core

layout (location = 0) in vec3 vertPosition;
uniform mat4 matrix;

void main() { gl_Position = matrix * vec4(vertPosition, 1.0); }
)";

    constexpr const char* fragmentShader = R"(
#version 330 core

out vec4 color;

void main() { color = vec4(1.0); }
)";
} // namespace

using namespace sgct;

void myDraw2DFun(RenderData data) {
#ifdef SGCT_HAS_TEXT
    text::print(
        data.window,
        data.viewport,
        *text::FontManager::instance().font("SGCTFont", 24),
        text::Alignment::TopLeft,
        100,
        500,
        glm::vec4(0.f, 1.f, 0.f, 1.f),
        "Time: %ls", sTimeOfDay.value().c_str()
    );
    if (extraPackages.value() && extraData.size() == ExtendedSize) {
        float xp = data.window.framebufferResolution().x / 2.f - 150.f;
        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 16),
            text::Alignment::TopLeft,
            xp,
            150.f,
            glm::vec4(0.f, 1.f, 0.5f, 1.f),
            "Vector val: %f, size: %u",
            extraData.valueAt(ExtendedSize / 2), extraData.size()
        );
    }
#endif // SGCT_HAS_TEXT
}

void drawFun(RenderData data) {
    if (slowRendering.value()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // test quadbuffer
    if (frametest.value()) {
        if (Engine::instance().currentFrameNumber() % 2 == 0) {
            // even
            if (data.frustumMode == Frustum::Mode::StereoRightEye) {
                // left eye or mono since clear color is one step behind  -> red
                Engine::instance().setClearColor(glm::vec4(0.f, 0.f, 1.f, 1.f));
            }
            else {
                // right -> blue
                Engine::instance().setClearColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
            }
        }
        else {
            // odd
            if (data.frustumMode == Frustum::Mode::StereoRightEye) {
                // left eye or mono since clear color is one step behind
                Engine::instance().setClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
            }
            else {
                //right
                Engine::instance().setClearColor(glm::vec4(0.f, 1.f, 0.f, 1.f));
            }
        }
    }
    else {
        Engine::instance().setClearColor(glm::vec4(0.f, 0.f, 0.f, 0.f));
    }

    ShaderManager::instance().shaderProgram("simple").bind();
    glm::mat4 matrix = glm::rotate(
        data.modelViewProjectionMatrix,
        glm::radians(static_cast<float>(currentTime.value()) * speed.value()),
        glm::vec3(0.f, 1.f, 0.f)
    );
    matrix = glm::scale(matrix, glm::vec3(1.f, 0.5f, 1.f));
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));

    glBindVertexArray(geometry.vao);
    glDrawElements(GL_LINE_STRIP, nVertices, GL_UNSIGNED_BYTE, nullptr);
    glBindVertexArray(0);

#ifdef SGCT_HAS_TEXT
    float pos = data.window.framebufferResolution().x / 2.f;

    if (data.frustumMode == Frustum::Mode::StereoLeftEye) {
        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 32),
            text::Alignment::TopRight,
            pos,
            200,
            glm::vec4(1.f),
            "Left"
        );
    }
    else if (data.frustumMode == Frustum::Mode::StereoRightEye) {
        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 32),
            text::Alignment::TopLeft,
            pos,
            150,
            glm::vec4(1.f),
            "Right"
        );
    }
    else if (data.frustumMode == Frustum::Mode::MonoEye) {
        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 32),
            text::Alignment::TopLeft,
            pos,
            200,
            glm::vec4(1.f),
            "Mono"
        );
    }

    if (data.window.isUsingSwapGroups()) {
        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 18),
            text::Alignment::TopLeft,
            pos - pos / 2.f,
            450,
            glm::vec4(1.f),
            "Swap group: Active"
        );

        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 18),
            text::Alignment::TopLeft,
            pos - pos / 2.f,
            500,
            glm::vec4(1.f),
            "Press B to toggle barrier and R to reset counter"
        );

        if (data.window.isBarrierActive()) {
            text::print(
                data.window,
                data.viewport,
                *text::FontManager::instance().font("SGCTFont", 18),
                text::Alignment::TopLeft,
                pos - pos / 2.f,
                400,
                glm::vec4(1.f),
                "Swap barrier: Active"
            );
        }
        else {
            text::print(
                data.window,
                data.viewport,
                *text::FontManager::instance().font("SGCTFont", 18),
                text::Alignment::TopLeft,
                pos - pos / 2.f,
                400,
                glm::vec4(1.f),
                "Swap barrier: Inactive"
            );
        }

        if (data.window.isSwapGroupMaster()) {
            text::print(
                data.window,
                data.viewport,
                *text::FontManager::instance().font("SGCTFont", 18),
                text::Alignment::TopLeft,
                pos - pos / 2.f,
                350,
                glm::vec4(1.f),
                "Swap group master: True"
            );
        }
        else {
            text::print(
                data.window,
                data.viewport,
                *text::FontManager::instance().font("SGCTFont", 18),
                text::Alignment::TopLeft,
                pos - pos / 2.f,
                350,
                glm::vec4(1.f),
                "Swap group master: False"
            );
        }

        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 18),
            text::Alignment::TopLeft,
            pos - pos / 2.f,
            300,
            glm::vec4(1.f),
            "Nvidia frame counter: %u", data.window.swapGroupFrameNumber()
        );
        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 18),
            text::Alignment::TopLeft,
            pos - pos / 2.f,
            250,
            glm::vec4(1.f),
            "Framerate: %.3lf", 1.0 / Engine::instance().statistics().dt()
        );
    }
    else {
        text::print(
            data.window,
            data.viewport,
            *text::FontManager::instance().font("SGCTFont", 18),
            text::Alignment::TopLeft,
            pos - pos / 2.f,
            450,
            glm::vec4(1.f),
            "Swap group: Inactive"
        );
    }
#endif // SGCT_HAS_TEXT
}

void preSyncFun() {
    if (Engine::instance().isMaster()) {
        currentTime.setValue(Engine::instance().getTime());

        time_t now = time(nullptr);
        constexpr const int TimeBufferSize = 256;
        char TimeBuffer[TimeBufferSize];
#if (_MSC_VER >= 1400) //visual studio 2005 or later
        tm timeInfo;
        errno_t err = localtime_s(&timeInfo, &now);
        if (err == 0) {
            strftime(TimeBuffer, TimeBufferSize, "%X", &timeInfo);
        }
#else
        tm* timeInfoPtr;
        timeInfoPtr = localtime(&now);
        strftime(TimeBuffer, TimeBufferSize, "%X", timeInfoPtr);
#endif
        const std::string time = TimeBuffer;
        const std::wstring wTime(time.begin(), time.end());
        sTimeOfDay.setValue(wTime);
    }
}

void postSyncPreDrawFun() {
    // barrier is set by swap group not window both windows has the same HDC
    Window::setBarrier(barrier.value());
    Engine::instance().setStatsGraphVisibility(stats.value());

    if (takeScreenshot.value()) {
        Engine::instance().takeScreenshot();
        takeScreenshot.setValue(false);
    }
}

void initOGLFun(GLFWwindow*) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    size_t numberOfActiveViewports = 0;
    const Node& thisNode = ClusterManager::instance().thisNode();
    for (const std::unique_ptr<Window>& window : thisNode.windows()) {
        for (const std::unique_ptr<Viewport>& vp : window->viewports()) {
            if (vp->isEnabled()) {
                numberOfActiveViewports++;
            }
        }
    }

    Log::Info("Number of active viewports: %d", numberOfActiveViewports);

    constexpr const uint8_t RestartIndex = std::numeric_limits<uint8_t>::max();

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RestartIndex);

    glGenVertexArrays(1, &geometry.vao);
    glGenBuffers(1, &geometry.vbo);
    glGenBuffers(1, &geometry.ibo);

    glBindVertexArray(geometry.vao);
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ibo);

    struct Vertex {
        float x, y, z;
    };
    std::vector<Vertex> vertices;
    std::vector<uint8_t> indices;

    vertices.push_back({ -1.f, -1.f, -1.f });     // 0: ---
    vertices.push_back({  1.f, -1.f, -1.f });     // 1: +--
    vertices.push_back({  1.f, -1.f,  1.f });     // 2: +-+
    vertices.push_back({ -1.f, -1.f,  1.f });     // 3: --+
    vertices.push_back({ -1.f,  1.f, -1.f });     // 4: -+-
    vertices.push_back({  1.f,  1.f, -1.f });     // 5: ++-
    vertices.push_back({  1.f,  1.f,  1.f });     // 6: +++
    vertices.push_back({ -1.f,  1.f,  1.f });     // 7: -++


    // bottom
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(RestartIndex);

    // top
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(4);
    indices.push_back(RestartIndex);

    // sides
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(RestartIndex);
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(RestartIndex);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(RestartIndex);
    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(RestartIndex);

    nVertices = static_cast<int>(indices.size());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        nullptr
    );
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(uint8_t),
        indices.data(),
        GL_STATIC_DRAW
    );
    glBindVertexArray(0);

    ShaderManager::instance().addShaderProgram("simple", vertexShader, fragmentShader);
    const ShaderProgram& prog = ShaderManager::instance().shaderProgram("simple");
    prog.bind();
    matrixLocation = glGetUniformLocation(prog.id(), "matrix");
    prog.unbind();
}

void encodeFun() {
    unsigned char flags = 0;
    flags = extraPackages.value()  ? flags | 2   : flags & ~2;   // bit 2
    flags = barrier.value()        ? flags | 4   : flags & ~4;   // bit 3
    flags = stats.value()          ? flags | 8  : flags & ~8;  // bit 4
    flags = takeScreenshot.value() ? flags | 16  : flags & ~16;  // bit 5
    flags = slowRendering.value()  ? flags | 32  : flags & ~32;  // bit 6
    flags = frametest.value()      ? flags | 64 : flags & ~64; // bit 7

    SharedUChar sf(flags);

    SharedData::instance().writeDouble(currentTime);
    SharedData::instance().writeFloat(speed);
    SharedData::instance().writeUChar(sf);
    SharedData::instance().writeWString(sTimeOfDay);

    if (extraPackages.value()) {
        SharedData::instance().writeVector(extraData);
    }
}

void decodeFun() {
    SharedUChar sf;
    SharedData::instance().readDouble(currentTime);
    SharedData::instance().readFloat(speed);
    SharedData::instance().readUChar(sf);
    SharedData::instance().readWString(sTimeOfDay);

    unsigned char flags = sf.value();
    extraPackages.setValue(flags & 2);
    barrier.setValue(flags & 4);
    stats.setValue(flags & 8);
    takeScreenshot.setValue(flags & 16);
    slowRendering.setValue(flags & 32);
    frametest.setValue(flags & 64);

    if (extraPackages.value()) {
        SharedData::instance().readVector(extraData);
    }
}

void keyCallback(Key key, Modifier, Action action, int) {
    if (Engine::instance().isMaster()) {
        switch (key) {
            case Key::Esc:
                if (action == Action::Press) {
                    Engine::instance().terminate();
                }
                break;
            case Key::F:
                if (action == Action::Press) {
                    frametest.setValue(!frametest.value());
                }
                break;
            case Key::E:
                if (action == Action::Press) {
                    extraPackages.setValue(!extraPackages.value());
                }
                break;
            case Key::B:
                if (action == Action::Press) {
                    barrier.setValue(!barrier.value());
                }
                break;
            case Key::S:
                if (action == Action::Press) {
                    stats.setValue(!stats.value());
                }
                break;
            case Key::G:
                if (action == Action::Press) {
                    std::string m = "Testing!!\r\n";
                    if (NetworkManager::instance().externalControlConnection()) {
                        NetworkManager::instance().externalControlConnection()->sendData(
                            m.c_str(),
                            static_cast<int>(m.size())
                        );
                    }
                }
                break;
            case Key::F9:
                if (action == Action::Press) {
                    slowRendering.setValue(!slowRendering.value());
                }
                break;
            case Key::F10:
                if (action == Action::Press) {
                    takeScreenshot.setValue(true);
                }
                break;
            case Key::Up:
                speed.setValue(speed.value() * 1.1f);
                break;
            case Key::Down:
                speed.setValue(speed.value() / 1.1f);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    std::vector<std::string> arg(argv + 1, argv + argc);
    Configuration config = parseArguments(arg);
    config::Cluster cluster = loadCluster(config.configFilename);

    Engine::instance().setClearColor(glm::vec4(0.f, 0.f, 0.f, 0.f));
    Engine::Callbacks callbacks;
    callbacks.initOpenGL = initOGLFun;

    callbacks.keyboard = keyCallback;
    callbacks.draw2D = myDraw2DFun;
    callbacks.encode = encodeFun;
    callbacks.decode = decodeFun;
    callbacks.draw = drawFun;
    callbacks.preSync = preSyncFun;
    callbacks.postSyncPreDraw = postSyncPreDrawFun;

    try {
        Engine::create(cluster, callbacks, config);
    }
    catch (const std::runtime_error& e) {
        Log::Error("%s", e.what());
        Engine::destroy();
        return EXIT_FAILURE;
    }

    if (Engine::instance().isMaster()) {
        for (int i = 0; i < ExtendedSize; i++) {
            extraData.addValue(static_cast<float>(rand() % 500) / 500.f);
        }
    }

    Engine::instance().render();
    Engine::destroy();
    exit(EXIT_SUCCESS);
}
