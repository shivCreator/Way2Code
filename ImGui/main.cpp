#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <algorithm>
#include <queue>
#include <map>
using namespace std;

using namespace sf;

struct Response{
  static const int Yes = 1;
  static const int No = 0;
  static const int Open = 1;
  static const int Close = 0;
};

struct ImGui_Window{
  static ImVec2 offset;
  static ImVec2 boundary;
  bool isCollapsed=false;
  bool isFocused=true;
  bool isOpen;
  std::string title;
  ImVec2 size;
  ImVec2 position;

  ImGui_Window(std::string title="New Window", bool open=false, ImVec2 size=ImVec2(0,0), ImVec2 position=ImVec2(0,0))
  : isOpen(open), title(title), size(size), position(position)
  {}
};
ImVec2 ImGui_Window::offset(8, 27);
ImVec2 ImGui_Window::boundary(20, 40);

struct Brush{
  ImVec2 position;
  ImVec2 size;
  Color color;
  Texture tIcon;
  Sprite sIcon;
  bool is_selected;
  Brush(ImVec2 position=ImVec2(0,0), ImVec2 size=ImVec2(10,10), Color color=Color::Red, bool selected=false)
  :position(position), size(size), color(color), is_selected(selected)
  {}
  void setIcon(string iconSrc){
    tIcon.loadFromFile(iconSrc);
    size = tIcon.getSize();
    sIcon.setTexture(tIcon);
  }
  const Sprite getIcon(void){
    return sIcon;
  }
};

// function-helper-coordinate-change
Vector2i apply_pixel_steps(const ImVec2& position, const ImVec2& step){
  Vector2i res(position.x/step.x, position.y/step.y);
  res.x *= step.x;
  res.y *= step.y;
  return res;
}
int sign(int x){return (x < 0? -1:1);}
void flood_fill(RenderTexture& screen, RectangleShape& pixel, const ImVec2& position, const Color& replacement){
  Image canvas = screen.getTexture().copyToImage();
  Color target = canvas.getPixel(position.x + pixel.getSize().x/2, position.y + pixel.getSize().y/2);
  pixel.setFillColor(replacement);
  queue<ImVec2> Q;
  Q.push(position);
  while(!Q.empty()){
    ImVec2 edge_piece = Q.front();
    Q.pop();

    pixel.setPosition(edge_piece);
    screen.draw(pixel);

    cout << Q.size() << endl;
    ImVec2 neighbours[] = {
      ImVec2(-1, 0), /* left */
      ImVec2( 1, 0), /* right */
      ImVec2( 0,-1), /* top */
      ImVec2( 0, 1)  /* bottom */
    };

    canvas = screen.getTexture().copyToImage();

    for(auto& neighbour : neighbours){
      neighbour.x = edge_piece.x + neighbour.x * pixel.getSize().x;
      neighbour.y = edge_piece.y + neighbour.y * pixel.getSize().y;

      // out of bounds
      if(neighbour.x<0 || neighbour.x>canvas.getSize().x ||
         neighbour.y<0 || neighbour.y>canvas.getSize().y)
        continue;
      // current color not to be replaced or same as fill color
      else if(canvas.getPixel(neighbour.x+pixel.getSize().x/2, neighbour.y-pixel.getSize().y/2) != target 
           || canvas.getPixel(neighbour.x+pixel.getSize().x/2, neighbour.y-pixel.getSize().y/2) == replacement)
        continue;
      Q.push(neighbour);
    }
  }
}

int main(int argc, char** argv)
{
  RenderWindow window(VideoMode(800, 600), "Gui Paint");
  ImGui::SFML::Init(window);

  Image app_icon;
  app_icon.loadFromFile("res/img/app_icon.png");

  window.setIcon(app_icon.getSize().x, app_icon.getSize().y, app_icon.getPixelsPtr());

  Event event;

  Clock clock;

  /*    Resources   */
  float brush_color[] = {1, 0, 0, 1};

  // resource-dialog-offset
  ImVec2 dialog_offset(100, 100);

  // resource-canvas
  ImGui_Window Canvas;
  Canvas.title = "New Canvas";
  Canvas.isOpen = false;
  Canvas.size = ImVec2(50, 50);
  Canvas.position = ImVec2(50, 30);
  Canvas.isCollapsed = false;

  // resource-buffer
  const Color canvas_bgColor(198, 204, 224);
  const Color color_grid(Color::White);
  RenderTexture canvas_buffer;
  Texture tGrid;

  // resource-toolkit
  vector<Brush*> toolkit;
  
  // resource-pixel
  RectangleShape pixel(Vector2f(10, 10)); // pixel
  pixel.setFillColor(canvas_bgColor);     // of backgroud color // Transparent

  // resource-brush-draw
  Brush draw_brush(ImVec2(0,0), ImVec2(pixel.getSize().x, pixel.getSize().y), Color::Red);
  draw_brush.setIcon("res/img/draw_brush_icon.png");
  toolkit.push_back(&draw_brush);
  
  // resource-eraser
  Brush eraser(ImVec2(0,0), ImVec2(2*pixel.getSize().x, 2*pixel.getSize().y), canvas_bgColor);
  eraser.setIcon("res/img/eraser_icon.png");
  toolkit.push_back(&eraser);
  
  // resource-color-bucket
  Brush color_bucket(ImVec2(0,0), ImVec2(pixel.getSize().x, pixel.getSize().y), Color::Red);
  color_bucket.setIcon("res/img/color_bucket_icon.png");
  toolkit.push_back(&color_bucket);
  
  // resource-dropper
  Brush dropper(ImVec2(0,0), ImVec2(pixel.getSize().x, pixel.getSize().y), Color::Red);
  dropper.setIcon("res/img/dropper_icon.png");
  toolkit.push_back(&dropper);
  
  // resource-brush-pointer
  Brush* brush_pointer = nullptr;

  // resource-menu
  map<string, bool> Menu;

  // resource-file
  map<string, string> File;
  File["name"] = "Name";
  File["location"] = "Current Location";

  // resource-helper-bool
  map<string, bool> bHelper;
  // resource-helper-int
  map<string, int> nHelper;

  // Application Loop
  while(window.isOpen()){
    while(window.pollEvent(event)){
      ImGui::SFML::ProcessEvent(event);

      // event-close
      if(event.type == Event::Closed || Menu["exit"])
        window.close();
    }
/*      Update-Section     */
    // Update ImGui
    ImGui::SFML::Update(window, clock.restart());

    // Handle Flags
    if(Menu["draw-brush"]){
      Menu["draw-brush"] = false; 
      // Set all brushes as not-selected
      for(auto& brush : toolkit) brush->is_selected = false;

      // Mark draw brush selected
      draw_brush.is_selected = true;
    }
    else if(Menu["eraser"]){
      Menu["eraser"] = false;
      // Set all brushes as not selected
      for(auto& brush : toolkit) brush->is_selected = false;

      // Mark eraser as selected
      eraser.is_selected = true;
    }
    else if(Menu["color-bucket"]){
      Menu["color-bucket"] = false;
      // Set all brushes as not selected
      for(auto& brush : toolkit) brush->is_selected = false;

      // Mark color-bucket as selected
      color_bucket.is_selected = true;
      color_bucket.color.r = 255*brush_color[0];
      color_bucket.color.g = 255*brush_color[1];
      color_bucket.color.b = 255*brush_color[2];
      color_bucket.color.a = 255*brush_color[3];
    }
    else if(Menu["dropper"]){
      Menu["dropper"] = false;
      // Set all brushes as not selected
      for(auto& brush : toolkit) brush->is_selected = false;

      // Mark eraser as selected
      dropper.is_selected = true;
    }

    // Keyboard Events
    // Left Control Pressed
    if( Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl) ){
      // keyboard-event-exit
      if(Keyboard::isKeyPressed(Keyboard::Q)){  Menu["exit"] = true; }
      // keyboard-event-new
      if(Keyboard::isKeyPressed(Keyboard::N)){  Menu["new"]  = true; }
      // keyboard-event-save
      if(Keyboard::isKeyPressed(Keyboard::S) && Canvas.isOpen){  Menu["save"] = true; }
      // keyboard-event-open
      if(Keyboard::isKeyPressed(Keyboard::O)){  Menu["open"] = true; }
      // Keyboard-event-select-draw-brush
      if(Keyboard::isKeyPressed(Keyboard::B) && Canvas.isOpen){  Menu["draw-brush"] = true;  }
      // Keyboard-event-select-eraser
      if(Keyboard::isKeyPressed(Keyboard::E) && Canvas.isOpen){  Menu["eraser"] = true;  }
      // Keyboard-event-select-color-bucket
      if(Keyboard::isKeyPressed(Keyboard::F) && Canvas.isOpen){  Menu["color-bucket"] = true;  }
      // Keyboard-event-select-dropper
      if(Keyboard::isKeyPressed(Keyboard::D) && Canvas.isOpen){  Menu["dropper"] = true;  }
    }

    // Mouse Events
    auto mouse_position = Mouse::getPosition(window);

    // move brush pointer along with the mouse
    if(brush_pointer){
      brush_pointer->position = ImVec2(mouse_position.x, mouse_position.y);
    }
    // if Canvas created and inside canvas
    if(mouse_position.x > Canvas.position.x + ImGui_Window::offset.x && mouse_position.x < Canvas.position.x + Canvas.size.x &&
       mouse_position.y > Canvas.position.y + ImGui_Window::offset.y && mouse_position.y < Canvas.position.y + Canvas.size.y &&
       Canvas.isOpen && !Canvas.isCollapsed && Canvas.isFocused)
    {
      for(auto& brush : toolkit)
        if(brush->is_selected)
          brush_pointer = brush;
    }
    else
    {
      brush_pointer = nullptr;
    }

    // Left Click Pressed
    if( Mouse::isButtonPressed(Mouse::Left) )
    {
      // Canvas created and a brush selected
      if(Canvas.isOpen && Canvas.isFocused && brush_pointer){
        // Window coordinates to canvas coordinates
        Vector2i brush_position;
        brush_position = Vector2i(brush_pointer->position.x, brush_pointer->position.y); 
        brush_position.x -= Canvas.position.x + ImGui_Window::offset.x;
        brush_position.y -= Canvas.position.y + ImGui_Window::offset.y;

        // canvas coordinates to integral steps
        brush_position.x = ( brush_position.x / (int)pixel.getSize().x ) * pixel.getSize().x;

        brush_position.y = ( brush_position.y / (int)pixel.getSize().y ) * pixel.getSize().y;

        Vector2i brush_position_flipped(brush_position);
        // @info : canvas-buffer's texture gets flipped when drawn on the screen 
        // flip-vertically // correct flip caused by drawing to canvas_buffer
        brush_position_flipped.y = canvas_buffer.getSize().y - brush_position.y - pixel.getSize().y;

        // mouse-event-draw
        if(draw_brush.is_selected){
          pixel.setPosition( Vector2f(brush_position_flipped.x, brush_position_flipped.y) );
          pixel.setFillColor(brush_pointer->color);

          canvas_buffer.display();  // @change
          canvas_buffer.draw(pixel);
        }
        // mouse-event-erase
        else if(eraser.is_selected)
        {
          pixel.setFillColor( canvas_bgColor ); // @change
          Vector2i eraserUL, eraserUR, eraserDL, eraserDR;
          // eraser-corner-upper-left
          eraserUL = apply_pixel_steps(ImVec2(brush_position_flipped.x-eraser.size.x/2, brush_position_flipped.y-eraser.size.y/2), pixel.getSize());
          // eraser-corner-upper-right
          eraserUR = apply_pixel_steps(ImVec2(brush_position_flipped.x+eraser.size.x/2, brush_position_flipped.y-eraser.size.y/2), pixel.getSize());
          // eraser-corner-down-left
          eraserDL = apply_pixel_steps(ImVec2(brush_position_flipped.x-eraser.size.x/2, brush_position_flipped.y+eraser.size.y/2), pixel.getSize());
          // eraser-corner-down-right
          eraserDR = apply_pixel_steps(ImVec2(brush_position_flipped.x+eraser.size.x/2, brush_position_flipped.y+eraser.size.y/2), pixel.getSize());

          Vector2i corners[] = {eraserUL, eraserUR, eraserDR, eraserDL, eraserUL};
          int size = sizeof(corners)/sizeof(Vector2i);
          // move around corners clockwise
          for(int i=0;i < size-1; i++){
            auto start= corners[i];
            auto end  = corners[i+1];


            // draw boundaries
            int dx = end.x-start.x;
            int dy = end.y-start.y;
            int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
            steps = steps / pixel.getSize().x;

            int xi = (dx? sign(dx)*pixel.getSize().x:0);
            int yi = (dy? sign(dy)*pixel.getSize().y:0);

            pixel.setPosition(Vector2f(start.x, start.y));
            window.draw(pixel);

            for(int i = 0; i < steps; i++){
              start.x += xi;
              start.y += yi;
              pixel.setPosition(Vector2f(start.x, start.y));
              canvas_buffer.display(); // @change
              canvas_buffer.draw(pixel);
            }
          }
        }
        // mouse-event-color-bucket @change
        else if(color_bucket.is_selected)
        {
          canvas_buffer.display();
          Texture texture = canvas_buffer.getTexture();
          Image screen = texture.copyToImage();
          std::queue<Vector2i> Q;

          int px = pixel.getSize().x;
          int py = pixel.getSize().y;
          int screenx = screen.getSize().x;
          int screeny = screen.getSize().y;

          Vector2i position;
          position.x = brush_position_flipped.x + px/2;
          position.y = brush_position_flipped.y + py/2;

          const Color target = screen.getPixel(position.x, position.y);
          const Color& replacement = color_bucket.color;
          Color color = target;

          auto plotPixel = [&](int x, int y){

            for(int Y = y-py/2; Y < y+py/2; Y++){
              for(int X = x-px/2; X < x+px/2; X++){
                screen.setPixel(X, Y, replacement);
              }
            }

          };

          if(target != replacement){
            plotPixel(position.x, position.y);

            Q.push(position);
            while(!Q.empty()){

              auto n = Q.front();
              Q.pop();
  
              ImVec2 neighbours[] = {
                ImVec2(-1, 0), /* left   */
                ImVec2( 1, 0), /* right  */
                ImVec2( 0,-1), /* top    */
                ImVec2( 0, 1)  /* bottm  */
              };
              for(auto& neighbour : neighbours){
                neighbour.x = n.x + neighbour.x * pixel.getSize().x;
                neighbour.y = n.y + neighbour.y * pixel.getSize().y;
  
                color = screen.getPixel(neighbour.x, neighbour.y);
                if(neighbour.x >= 0 && neighbour.y >= 0 && neighbour.x < screenx && neighbour.y < screeny
                                   && color == target)
                {
                  plotPixel(neighbour.x, neighbour.y);
                  Q.push(neighbour);
                }
              }
            }
            
            texture.loadFromImage(screen);
            canvas_buffer.clear();
            canvas_buffer.draw(Sprite(texture));
          }
        }
        // mouse-event-dropper
        else if(dropper.is_selected)
        {
          canvas_buffer.display();
          Texture texture = canvas_buffer.getTexture();
          Image screen = texture.copyToImage();
          Color color = screen.getPixel(brush_position_flipped.x + pixel.getSize().x/2, brush_position_flipped.y + pixel.getSize().y/2);

          brush_color[0] = (color.r / 255.f);
          brush_color[1] = (color.g / 255.f);
          brush_color[2] = (color.b / 255.f);
          brush_color[4] = (color.a / 255.f);

          draw_brush.color = color;
        }
      }
    }

    // Right Click Pressed
    else if(Mouse::isButtonPressed(Mouse::Right)){
      // mouse-event-nobrush
      for_each(toolkit.begin(), toolkit.end(), [](Brush* b){b->is_selected=false;});
      brush_pointer = nullptr;
    }

/*      Draw-Section      */
    // Draw ImGui windows

    /*  menu-bar  */
    if(ImGui::BeginMainMenuBar())
    {
      // menu-file
      if(ImGui::BeginMenu("File"))
      {
        // mouse-event-new
        if(ImGui::MenuItem("New", "Ctrl+N")){
          Menu["new"] = true;
        }
        // mouse-event-save
        else if(ImGui::MenuItem("Save", "Ctrl+S") && Canvas.isOpen){ Menu["save"] = true;}
        // mouse-event-open
        else if(ImGui::MenuItem("Open", "Ctrl+O")){ Menu["open"] = true; }
        // mouse-event-exit
        else if(ImGui::MenuItem("Exit", "Ctrl+Q")){ Menu["exit"] = true; }

        ImGui::EndMenu();
      }
      // menu-edit
      if(ImGui::BeginMenu("Edit"))
      { 
        /* Add Menu Items */
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
    
    /* window-toolbar */
    ImGui::SetNextWindowPos(ImVec2(0, 30), ImGuiCond_Once);
    if(ImGui::Begin("##PaintToolbar", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
      // tool-brush
      if(ImGui::ImageButton(draw_brush.tIcon)){
        Menu["draw-brush"] = true;
      }
      // tooltip
      if(ImGui::IsItemHovered()){
        ImGui::BeginTooltip();
          ImGui::SetTooltip("Draw-Brush(Ctrl+B)");
        ImGui::EndTooltip();
      }

      // tool-eraser
      if(ImGui::ImageButton(eraser.tIcon)){
        Menu["eraser"] = true;
      }
      // tooltip
      if(ImGui::IsItemHovered()){
        ImGui::BeginTooltip();
          ImGui::SetTooltip("Eraser(Ctrl+E)");
        ImGui::EndTooltip();
      }

      // tool-color-bucket
      if(ImGui::ImageButton(color_bucket.tIcon)){
        Menu["color-bucket"] = true;
      }
      // tooltip
      if(ImGui::IsItemHovered()){
        ImGui::BeginTooltip();
          ImGui::SetTooltip("Color-Bucket(Ctrl+F)");
        ImGui::EndTooltip();
      }

      // tool-dropper
      if(ImGui::ImageButton(dropper.tIcon)){
        Menu["dropper"] = true;
      }
      // tooltip
      if(ImGui::IsItemHovered()){
        ImGui::BeginTooltip();
          ImGui::SetTooltip("Dropper(Ctrl+D)");
        ImGui::EndTooltip();
      }

    }
      ImGui::End();

    /* window-color-picker*/
    ImGui::SetNextWindowSize(ImVec2(200, 220), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(window.getSize().x - 200, 30), ImGuiCond_Once);
    if(ImGui::Begin("Color-Picker")){
      if(ImGui::ColorPicker4("Color-Picker", brush_color)){
        draw_brush.color.r = 255 * brush_color[0];
        draw_brush.color.g = 255 * brush_color[1];
        draw_brush.color.b = 255 * brush_color[2];
        draw_brush.color.a = 255 * brush_color[3];
      }
    }
      ImGui::End();

    // window-new
    if( Menu["new"] ) {
      if(!bHelper["considered"])
      {
        // initial setup
        bHelper["considered"] = true;
        File["name"] = "image.png";
        File["location"] = "Current Location";
        File["width"] = "50";
        File["height"] = "50";
      }

      char filename[255];
      char width[4], height[4];
      // initialize
      strcpy(filename, File["name"].c_str());
      strcpy(width, File["width"].c_str());
      strcpy(height, File["height"].c_str());

      ImGui::SetNextWindowFocus();
      ImGui::SetNextWindowPos(ImVec2(window.getSize().x/2 - dialog_offset.x, window.getSize().y/2 - dialog_offset.y), ImGuiCond_Once);
      ImGui::Begin("New Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Enter Canvas Description");
        ImGui::InputText("Name", filename, 255);
        ImGui::InputText("Width(px)", width, 4);
        ImGui::InputText("Height(px)", height, 4);

        // save input
        File["name"] = filename;
        File["width"]  = width;
        File["height"] = height;

        // new-button-create
        if(ImGui::Button("Create")){
          // setup-flags
          // Close setup window
          Menu["new"] = false;
          bHelper["considered"] = false;
          bHelper["created"] = true;

          // convert new canvas dimensions to screen dimensions
          int nwidth, nheight;
          nwidth  = atoi(width)  * pixel.getSize().x + ImGui_Window::boundary.x;
          nheight = atoi(height) * pixel.getSize().y + ImGui_Window::boundary.x;

          // ImGui_Window(std::string title, bool open, ImVec2 size, ImVec2 position)
          Canvas.title  = File["name"];
          Canvas.isOpen = true;
          Canvas.position = ImVec2(50, 30);
          Canvas.size.x = (nwidth  > window.getSize().x-250) ? window.getSize().x-250 : nwidth;
          Canvas.size.y = (nheight > window.getSize().y- 50) ? window.getSize().y- 50 : nheight;

          canvas_buffer.create(nwidth - ImGui_Window::boundary.x, nheight - ImGui_Window::boundary.y);
          canvas_buffer.clear(canvas_bgColor);

          // draw pixel grid
          Image img_grid;
          img_grid.create(canvas_buffer.getSize().x, canvas_buffer.getSize().y, Color::Transparent);

          for(int y = 0; y < img_grid.getSize().y; y++)
          {
            for(int x = 0; x < img_grid.getSize().x; x++)
            {
              if(x%(int)pixel.getSize().x==0 || y%(int)pixel.getSize().y==0)
                img_grid.setPixel(x, y, color_grid);
            }
          }

          tGrid.loadFromImage(img_grid);
          canvas_buffer.draw( Sprite( tGrid ) );
        }
        // new-button-cancel
        else if(ImGui::SameLine(), ImGui::Button("Cancel")){
          Menu["new"] = false;  // Close Setup window
          bHelper["considered"] = false;
        }

      ImGui::End();
    }

    // window-save
    else if(Menu["save"] && Canvas.isOpen){
      if(!bHelper["considered"]){
        bHelper["considered"] = true;
        File["name"] = Canvas.title;
        File["location"] = "Current Location";
      }
      ImGui::SetNextWindowFocus();
      ImGui::SetNextWindowPos(ImVec2(window.getSize().x/2 - dialog_offset.x, window.getSize().y/2 - dialog_offset.y), ImGuiCond_Once);
      if(ImGui::Begin("Save Image", nullptr, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoCollapse))
      {
        char filename[255];       strcpy(filename, File["name"].c_str());
        char file_location[255];  strcpy(file_location, File["location"].c_str());
        ImGui::InputText("Name", filename, 255);
        ImGui::InputText("Location", file_location, 255);

        File["name"] = filename;
        File["location"] = file_location;

        // save-button-save
        if(ImGui::Button("Save"))
        {
          //  Reset states
          Menu["save"] = false;
          bHelper["considered"] = false;

          if(!(File["location"] == "Current Location")){
            File["name"] = File["location"] + "/" + File["name"];
          }

          canvas_buffer.display();  // @change
          Image tempImg = canvas_buffer.getTexture().copyToImage();
          Image img_to_save;
          img_to_save.create(tempImg.getSize().x / pixel.getSize().x, tempImg.getSize().y / pixel.getSize().y, Color::Transparent);

          for(int y=pixel.getSize().y/2; y < tempImg.getSize().y; y+=pixel.getSize().y){
            for(int x=pixel.getSize().x/2; x < tempImg.getSize().x; x+=pixel.getSize().y){
              if(tempImg.getPixel(x, y) == canvas_bgColor)
                continue;
              else
                img_to_save.setPixel(x/pixel.getSize().x, y/pixel.getSize().y, tempImg.getPixel(x, y));
            }
          }

          img_to_save.flipVertically();
          img_to_save.saveToFile(File["name"]);
        }
        // save-button-cancel
        else if(ImGui::SameLine(), ImGui::Button("Cancel"))
        {
          Menu["save"] = false;
          bHelper["considered"] = false;
        }
      }
        ImGui::End();
    }
    else if(Menu["open"])
    { 
      // window-alert
      if(Canvas.isOpen && !bHelper["considered"]){
        ImGui::SetNextWindowFocus();
        ImGui::SetNextWindowPos(ImVec2(window.getSize().x/2 - dialog_offset.x, window.getSize().y/2 - dialog_offset.y), ImGuiCond_Once);
        ImGui::Begin("Alert", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
          ImGui::TextUnformatted("Do you want to save your work?");
          if(ImGui::Button("Yes")){
            // Save First
            Menu["save"] = true;
            bHelper["considered"] = true;
          }
          else if(ImGui::SameLine(), ImGui::Button("No")){
            // Initial setting before opening image
            File["name"] = "image.png";
            File["location"] = "Current Location";
            bHelper["considered"] = true;
          }
        ImGui::End();

      }
      else
      {
        // Setup Initial states
        if(!bHelper["considered"]){
          bHelper["considered"] = true;
          File["name"] = "image.png";
          File["location"] = "Current Location";
        }
        // window-open
        ImGui::SetNextWindowFocus();
        ImGui::SetNextWindowPos(ImVec2(window.getSize().x/2 - dialog_offset.x, window.getSize().y/2 - dialog_offset.y), ImGuiCond_Once);
        if(ImGui::Begin("Open Image"), nullptr, ImGuiWindowFlags_AlwaysAutoResize)
        {
          char filename[255], file_location[255];
          // initialize inputs
          strcpy(filename, File["name"].c_str());
          strcpy(file_location, File["location"].c_str());
          // take input
          ImGui::InputText("Name", filename, 255);
          ImGui::InputText("Location", file_location, 255);
          // save input
          File["name"] = filename;
          File["location"] = file_location;
          // open-button-open
          if(ImGui::Button("Open")){
            // setup-flags
            // Reset States
            Menu["open"] = false;
            bHelper["considered"] = false;
            bHelper["created"] = true;

            // Setup Canvas
            Canvas.title = File["name"];
            Canvas.isOpen = true;

            if( !(File["location"]=="Current Location") )
            {
              File["name"] = File["location"]+"/"+File["name"];
            }

            // Open Image
            Image opened_image;
            opened_image.loadFromFile(File["name"]);
            opened_image.flipVertically();

            // reset buffers
            Canvas.size.x = opened_image.getSize().x*pixel.getSize().x + ImGui_Window::boundary.x;
            Canvas.size.y = opened_image.getSize().y*pixel.getSize().y + ImGui_Window::boundary.y;
            Canvas.position = ImVec2(50, 30);

            canvas_buffer.create(Canvas.size.x - ImGui_Window::boundary.x, Canvas.size.y - ImGui_Window::boundary.y);

            // load image onto canvas_buffer
            for(int y=pixel.getSize().y/2; y < canvas_buffer.getSize().y; y += pixel.getSize().y)
            {
              for(int x=pixel.getSize().x/2; x < canvas_buffer.getSize().x; x += pixel.getSize().x)
              {
                Color fillColor = opened_image.getPixel(x/pixel.getSize().x, y/pixel.getSize().y);
                
                if(fillColor == Color::Transparent) fillColor = canvas_bgColor;

                pixel.setFillColor(fillColor);
                pixel.setPosition(Vector2f(x-pixel.getSize().x/2, y-pixel.getSize().y/2));

                canvas_buffer.display();
                canvas_buffer.draw(pixel);
              }
            }

            // reset screen buffer or pixel grid
            Image img_grid;
            img_grid.create(canvas_buffer.getSize().x, canvas_buffer.getSize().y, Color::Transparent);

            for(int y = 0; y < canvas_buffer.getSize().y; y ++)
            {
              for(int x = 0; x < canvas_buffer.getSize().x; x++)
              {
                if(x%(int)pixel.getSize().x==0 || y%(int)pixel.getSize().y==0)
                  img_grid.setPixel(x, y, color_grid);
              }
            }

            // Texture tempTexture;
            tGrid.loadFromImage(img_grid);
            canvas_buffer.draw( Sprite( tGrid ) );
          }
          //open-button-cancel
          else if(ImGui::SameLine(), ImGui::Button("Cancel"))
          {
            Menu["open"] = false;
            bHelper["considered"] = false;
          }
        }
          ImGui::End();
      }
    }

    // window-debug
    // ImGui::SetNextWindowPos (ImVec2(window.getSize().x - 200, window.getSize().y - 200), ImGuiCond_Once);
    // if(ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    // {
    // }
    //   ImGui::End();

    // window-canvas
    if(Canvas.isOpen){

      int canvas_window_flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      if(mouse_position.x > Canvas.position.x && mouse_position.x < Canvas.position.x + Canvas.size.x &&
         mouse_position.y > Canvas.position.y && mouse_position.y < Canvas.position.y + ImGui_Window::offset.y)
      {
        canvas_window_flag = ImGuiWindowFlags_NoResize;
      }
      // @incomplete
      if(Canvas.size.x + ImGui_Window::boundary.x != canvas_buffer.getSize().x || 
         Canvas.size.y + ImGui_Window::boundary.y != canvas_buffer.getSize().y){
        canvas_window_flag |= ImGuiWindowFlags_HorizontalScrollbar;
      }

      if(bHelper["created"]){
        ImGui::SetNextWindowPos(Canvas.position);
        bHelper["created"] = false;
      }
      ImGui::SetNextWindowSize(Canvas.size, ImGuiCond_Always);
      ImGui::SetNextWindowCollapsed(false, ImGuiCond_Appearing);
      if(ImGui::Begin(Canvas.title.c_str(), &(Canvas.isOpen), canvas_window_flag))
      {
        // Update Canvas States
        Canvas.position = ImGui::GetWindowPos();
        Canvas.isCollapsed = false;
        Canvas.isFocused = ImGui::IsWindowFocused();

        // Draw inside window
        Sprite sGrid(tGrid);
        sGrid.setPosition(0, 0);
        canvas_buffer.draw(sGrid);
        canvas_buffer.display();
        ImGui::Image(canvas_buffer.getTexture());
      }
      else if(Canvas.isOpen)
      {
        Canvas.isCollapsed = true;
      }
        ImGui::End();
    }
    else 
      brush_pointer = nullptr;

    // clear window
    window.clear();

    // Show ImGui Windows
    ImGui::SFML::Render(window);

    // draw-pointer-icon
    if(brush_pointer)
    {
      brush_pointer->sIcon.setPosition(Vector2f(brush_pointer->position.x - brush_pointer->size.x / 2,
                                 brush_pointer->position.y - brush_pointer->size.y / 2));
      
      window.draw(brush_pointer->getIcon());
    }

    // display window
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}