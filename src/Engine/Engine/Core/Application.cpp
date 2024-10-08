
#include "Application.hpp"
#include "TimeModule.hpp"
#include "Debug/Log/Log.hpp"

namespace Ellipse
{

Application* Application::sInStance = nullptr;

Application::Application()
{
  sInStance = this;
}

void Application::init(const ApplicationSpecifications& specs)
{
  m_engine = Engine::createEngine();

  m_window = Window::createWindow(specs.m_windowSettings);
  m_window->setEventCallBack(BIND_EVENT_FN(Application::onEvent));


  // [ Create a render plugin with init data in the renderer hpp file ]
  auto renderPlugin = RenderPlugin::createRenderPlugin(specs.m_graphicSpec);
   
  m_renderer = Renderer::createRenderer(std::move(renderPlugin));
  Pair<int, int> winSize = m_window->getWindowSize();
  m_renderer->setWindowFrameSize(winSize);

  m_engine->addSystem(m_renderer);

  // [ Maybe shouldn't make it a time module ]
  // [ User might not need some modules, add 
  //   configurbility ]
  m_engine->addModule(TimeModule::createTimeModule());
  m_engine->addModule(RenderModule::createRenderModule(*m_engine));

  ForwardList<SharedPtr<Layer>> layers = specs.m_userFunc();

  for(auto i = layers.begin(); i != layers.end(); i++)
  {
  pushLayer(*i);
  }

  for(auto i = layers.begin(); i != layers.end(); i++)
  {
  (*i)->init();
  }
}

Application::~Application()
{

}
void Application::pushLayer(SharedPtr<Layer> layer)
{
    pushLayerToStack(m_layerStack, layer);
}

void Application::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatchEvent<WindowUserQuitEvent>(
    WINDOW_USER_QUIT_EVENT,
    BIND_EVENT_FN(onWindowClose)
                                                 );
    dispatcher.dispatchEvent<WindowResizeEvent>(
    WINDOW_RESIZED_EVENT,
    BIND_EVENT_FN(onWindowResize)
                                                 );
  
    updateLayerEvents(m_layerStack, e);
}

void Application::run()
{
    while(m_running)
    {
    m_renderer->clearColorBuffer();

    float dt = 0.0f;
    updateLayerStack(m_layerStack, dt);

    m_window->updateWindow();
    }
}

bool Application::onWindowClose(WindowUserQuitEvent& windowQuit)
{
    m_running = false;

    return true;
}

bool Application::onWindowResize(WindowResizeEvent& resizeEvent)
{
    m_renderer->setViewport(resizeEvent.width(), resizeEvent.height());
    m_renderer->setWindowFrameSize(Pair<i32_t, i32_t>{resizeEvent.width(), resizeEvent.height()});

    return true;
}

}    //Ellipse namespace
