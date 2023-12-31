#include "gameplay.hpp"
#include "gui.hpp"

// Hard Coded Enum Names
const std::string to_full_name(ResourceTypes p){
    switch(p)
    {
        case ResourceTypes::LUMBER: return "Lumber";
        case ResourceTypes::STONE: return "Stone";
        case ResourceTypes::WATER: return "Water";
        case ResourceTypes::GRASS: return "Grass";
        case ResourceTypes::CORN: return "Corn";
    
    }
    return ""; // or an empty string
    
}
const std::string to_full_string(WorkerStates p){return to_string(p);}

const std::string to_string(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return "L";
    case ResourceTypes::STONE: return "S";
    case ResourceTypes::WATER: return "W";
    case ResourceTypes::GRASS: return "G";
    case ResourceTypes::CORN: return "C";

  }
  return ""; // or an empty string
}
const sf::Color to_color(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return sf::Color::Red;
    case ResourceTypes::STONE: return sf::Color::Yellow;
    case ResourceTypes::WATER: return sf::Color::Blue;
    case ResourceTypes::GRASS: return sf::Color::Green;

  }
  return sf::Color::White;
}
const std::string to_string(ItemTypes p){
    switch(p)
    {
        case ItemTypes::NONE: return "";
    }
    return ""; // or an empty string
}
const std::string to_string(WorkerStates p){
  switch(p)
  {
    case WorkerStates::DEAD: return "dead";
    case WorkerStates::IDLE: return "idle";
    case WorkerStates::EXECUTINGTASK: return "executing task";
    case WorkerStates::MOVING: return "moving";
    case WorkerStates::GATHERIDLE: return "gatheridle";
    case WorkerStates::GATHERING: return "gathering";
    case WorkerStates::CONSTRUCTING: return "constructing";

  }
  return ""; // or an empty string
}
const std::string to_string(NeedsTypes p){
  switch(p)
  {
    case NeedsTypes::FOOD: return "food";
    case NeedsTypes::SLEEP: return "sleep";
    case NeedsTypes::WATER: return "water";

  }
  return ""; // or an empty string
}
const std::string to_string(BuildingTypes p){
  switch(p)
  {
    case BuildingTypes::NONE: return "";
    case BuildingTypes::WORKSPACE: return "Ws";
    case BuildingTypes::FARM: return "Fa";
  }
  return ""; // or an empty string
}
const std::string to_full_string(BuildingTypes p){
  switch(p)
  {
    case BuildingTypes::NONE: return "";
    case BuildingTypes::WORKSPACE: return "Workspace";
    case BuildingTypes::FARM: return "Farm";
  }
  return ""; // or an empty string
}
const std::string to_full_string(BuildingStatus p){
    switch(p)
    {
        case BuildingStatus::PRECONSTRUCTION: return "Preconstruction";
        case BuildingStatus::CONSTRUCTION: return "Construction";
        case BuildingStatus::READY: return "Ready";
        case BuildingStatus::OPERATING: return "Operating";
    }
    return ""; // or an empty string
}
const std::string to_full_string(ItemTypes p){
    switch(p)
    {
        case ItemTypes::NONE: return "";
        case ItemTypes::CORNSEED: return "Corn Seed";
        case ItemTypes::LUMBER: return "Lumber";
        case ItemTypes::STONE: return "Stone";
        case ItemTypes::WATER: return "Water";
        case ItemTypes::GRASS: return "Grass";
        case ItemTypes::CORN: return "Corn";
    }
    return ""; // or an empty string
}
const std::string to_full_string(RecipeTypes p){
    switch(p)
    {
        case RecipeTypes::NONE: return "";
        case RecipeTypes::PROCESSCORNSTALK: return "Process Cornstalk";
        case RecipeTypes::FARMCORN: return "Farm Corn";
    }
    return ""; // or an empty string
}
//////////////////////////////////////////////////////////////////////


////////////////////////////// GUI //////////////////////////////
void GUI::Update(double dt){
    // update all widgets
    for (auto& widget: widgets_){
        widget->Update(dt, mouse_x_, mouse_y_);
    }
}
void GUI::Draw(sf::RenderWindow& window){
    /*
    Layout: selected object status
    [----------------------------------------------]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----]
    [----------------------------------------------]
    left panel: selected object status
    */

    // nullptr protection
    if (map_ref_ == nullptr || viewport_ref_ == nullptr){
        std::cout << "map_ref_ or viewport_ref_ is nullptr" << std::endl;
        return;
    }

    // draw the gameplay
    sf::Font font;
    if (!font.loadFromFile("c:\\Windows\\Fonts\\arial.ttf"))
    {
        return;
    }

    // iterate through the viewport, size + 1 to draw the tiles on the edge
    for (int i=viewport_ref_->GetIntX(); i<viewport_ref_->GetIntWidth()+1; i++){
        for (int j=viewport_ref_->GetIntY(); j<viewport_ref_->GetIntHeight()+1; j++){
            // get the tile from map_ref_
            auto tile = map_ref_->GetTile(i, j);
            // skip if no tile (including OOB)
            if (tile == nullptr){
                continue;
            }       

            // get the string
            std::string str = "";
            sf::Color color(0, 0, 0);

            // check for building
            if (tile->building_type_ != BuildingTypes::NONE){
                str += to_string(tile->building_type_);
                color = sf::Color::White;
            } else {
                // iterate through resources
                for (auto& resource : map_ref_->GetTile(i, j)->resource_map_){
                    str += to_string(resource.second->resource_type_);
                    color = to_color(resource.second->resource_type_);
                }
            }
            if (str == ""){
                continue;
            }

            // calculate pixel position for i, j
            double pixelX = viewport_ref_->ConvertMeterToPixelX(window, i);
            double pixelY = viewport_ref_->ConvertMeterToPixelY(window, j);
            double pixelsPerMeterW = viewport_ref_->GetPixelsPerMeterX(window);
            double pixelsPerMeterH = viewport_ref_->GetPixelsPerMeterY(window);  

            // draw the tile
            sf::Text text;
            text.setFont(font);
            text.setString(str);
            text.setPosition(pixelX, pixelY);
            text.setOrigin(0, pixelsPerMeterH); // set origin to bottom left
            text.setCharacterSize(pixelsPerMeterH); // pixels
            // text.setScale(pixelsPerMeterW, pixelsPerMeterH); // isn't working well
            text.setFillColor(color);
            text.setStyle(sf::Text::Bold);
            window.draw(text);
        }
    }

    // iterate through dynamic game objects
    for (auto& dynamic_object_ptr : map_ref_->dynamic_object_ptrs_){
        // calculate pixel position for i, j
        double pixelX = viewport_ref_->ConvertMeterToPixelX(window, dynamic_object_ptr->footprint_.x_);
        double pixelY = viewport_ref_->ConvertMeterToPixelY(window, dynamic_object_ptr->footprint_.y_);   
        double pixelsPerMeterH = viewport_ref_->GetPixelsPerMeterY(window);         

        // draw the tile
        sf::Text text;
        text.setFont(font);
        text.setString("W");
        text.setPosition(pixelX, pixelY);
        text.setCharacterSize(pixelsPerMeterH); // pixels
        text.setOrigin(0, pixelsPerMeterH); // set origin to bottom left
        // text.setScale(pixelsPerMeterW, pixelsPerMeterH); // isn't working well
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        window.draw(text);
    }

    // HUD
    // HUD outline
    sf::RectangleShape shape(sf::Vector2f(300, window.getSize().y - 100));
    shape.setPosition(15.0, 110);
    shape.setFillColor(sf::Color::Black);
    // set a 10-pixel wide orange outline
    shape.setOutlineThickness(10.f);
    shape.setOutlineColor(sf::Color::Cyan);
    window.draw(shape);
    // end HUD outline

    // make string for selected unit
    std::string str = "";
    for (auto& dynamic_object_ptr : map_ref_->selected_dynamic_object_ptrs_){
        str += "Selected Unit: ";
        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
            auto worker = std::static_pointer_cast<Worker>(dynamic_object_ptr);
            str += "Worker";
            // add inventory
            str += "\nInventory: ";
            for (auto& item : worker->GetInventoryMap()){
                str += to_full_string(item.first);
                str += ": ";
                str += std::to_string(item.second->GetAmount());
                str += ", ";
            }
            // add state
            str += "\nState: ";
            str += to_string(worker->worker_state_);
            // if gathering, add gather progress
            if (worker->worker_state_ == WorkerStates::GATHERING){
                str += "\nGathering " + to_full_name(worker->selected_resource_ptr_->resource_type_) + " Progress: ";
                str += std::to_string(worker->action_time_) + " / " + std::to_string(worker->selected_resource_ptr_->time_to_gather_);
            }
            // if executing task, add task progress
            else if (worker->worker_state_ == WorkerStates::EXECUTINGTASK){
                if (worker->task_ptr_ != nullptr){
                    str += "\nExecuting Task Progress: \n";
                    for (int i=0; i<worker->task_ptr_->actions_.size(); i++){
                        if (i == worker->task_ptr_->active_action_){
                            str += ">> ";
                        }
                        str += worker->task_ptr_->actions_[i]->to_string() + "\n";
                    }
                }
            }
            // add needs
            str += "\nNeeds: ";
            for (auto& need : worker->needs_map_){
                str += to_string(need.first);
                str += ": ";
                str += std::to_string(need.second.val_);
                str += ", ";
            }

            // active task
            if (worker->task_ptr_ != nullptr){
                str += "\nActive task: " + worker->task_ptr_->task_name_;


            }


        } else if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::GENERIC){
            str += "Generic";
            break;
        }
        // add position
        str += "\nPos: " + std::to_string(dynamic_object_ptr->footprint_.x_);
        str += ", ";
        str += std::to_string(dynamic_object_ptr->footprint_.y_);
    }
    // end make string for selected unit

    // make string for selected tile
    if (map_ref_->selected_tile_ptr_ != nullptr){
        str += "Selected Tile: ";
        // add position
        str += "\nPos: " + std::to_string(map_ref_->selected_tile_ptr_->x_);
        str += ", ";
        str += std::to_string(map_ref_->selected_tile_ptr_->y_);
        // add resources
        str += "\nResources: ";
        for (auto& resource : map_ref_->selected_tile_ptr_->resource_map_){
            str += to_full_name(resource.second->resource_type_);
            str += ": ";
            str += std::to_string(resource.second->GetTotal());
            str += ", ";
        }
        // add building
        if (map_ref_->selected_tile_ptr_->building_type_ != BuildingTypes::NONE){
            str += "\nBuilding: ";
            str += to_full_string(map_ref_->selected_tile_ptr_->building_type_);
            str += "\nBuilding Status: " + to_full_string(map_ref_->selected_tile_ptr_->building_ptr_->GetStatus());
            str += "\nInventory: ";
            for (auto& item : map_ref_->selected_tile_ptr_->building_ptr_->inventory_map_){
                str += to_full_string(item.first);
                str += ": ";
                str += std::to_string(item.second);
                str += ", ";
            }
            // switch over building status
            switch (map_ref_->selected_tile_ptr_->building_ptr_->GetStatus()){
                case BuildingStatus::PRECONSTRUCTION:
                    // iterate through item_reqs_map
                    str += "\nItem Reqs: ";
                    for (auto& item : map_ref_->selected_tile_ptr_->building_ptr_->item_reqs_map_){
                        str += to_full_string(item.first);
                        str += ": ";
                        str += std::to_string(item.second);
                        str += ", ";
                    }
                    break;
                case BuildingStatus::CONSTRUCTION:
                    str += "\nConstruction Effort: " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->effort_val_);
                    str += " / " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->construction_effort_req_);
                    break;
                case BuildingStatus::READY:
                    str += "\n Recipe " + to_full_string(map_ref_->selected_tile_ptr_->building_ptr_->active_recipe_) + " Reqs: ";
                    for (auto& item : map_ref_->selected_tile_ptr_->building_ptr_->recipes_[map_ref_->selected_tile_ptr_->building_ptr_->active_recipe_].inputs_){
                        str += to_full_string(item.first);
                        str += ": ";
                        str += std::to_string(item.second);
                        str += ", ";
                    }
                    break;
                case BuildingStatus::OPERATING:
                    str += "\n Recipe Progress: " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->effort_val_);
                    str += " / " + std::to_string(map_ref_->selected_tile_ptr_->building_ptr_->recipes_[map_ref_->selected_tile_ptr_->building_ptr_->active_recipe_].effort_req_);
                    break;
            }

        }
    }
    
    // draw the string
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setPosition(20, 130);
    text.setCharacterSize(12); // pixels
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    window.draw(text);

    // draw all widgets
    for (auto& widget: widgets_){
        widget->draw(window, sf::Transform::Identity);
    }

}

GUIInputs GUI::HandleInput(sf::Event& event){
    // GUI level input handling
    GUIInputs output = GUIInputs::NONE;

    // iterate through widgets. Only 1 widget can handle an input so we can early exit
    for (auto& widget: widgets_){
        // handle input for each widget
        auto widget_input = widget->HandleInput(event);
        if (widget_input == WidgetInputs::HANDLED){
            return GUIInputs::HANDLED;
        }
    }
    return output;
}


ViewportInputs Viewport::HandleInput(sf::Event& event){
    // handle input for the title screen
    ViewportInputs output = ViewportInputs::NONE;

    switch (event.type){
        ///////////////// key pressed /////////////////
        case sf::Event::KeyPressed:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::A:
                    std::cout << "Move viewport left" << std::endl;
                    viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_LEFT);
                    output = ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::D:
                    std::cout << "Move viewport right" << std::endl;
                    viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_RIGHT);
                    output = ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::W:
                    std::cout << "Move viewport up" << std::endl;
                    viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_UP);
                    output = ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::S:
                    std::cout << "Move viewport down" << std::endl;
                    viewport_actions_.SetFlag((EFlagValue)ViewportActions::SCROLL_DOWN);
                    output = ViewportInputs::HANDLED;
                    break;
                default:
                    break;
            }
            break;
        ///////////////// key released /////////////////
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::A:
                    std::cout << "Stop moving viewport left" << std::endl;
                    viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_LEFT);
                    output = ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::D:
                    std::cout << "Stop moving viewport right" << std::endl;
                    viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_RIGHT);
                    output = ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::W:
                    std::cout << "Stop moving viewport up" << std::endl;
                    viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_UP);
                    output = ViewportInputs::HANDLED;
                    break;
                case sf::Keyboard::Scan::S:
                    std::cout << "Stop moving viewport down" << std::endl;
                    viewport_actions_.UnsetFlag((EFlagValue)ViewportActions::SCROLL_DOWN);
                    output = ViewportInputs::HANDLED;
                    break;
                    break;
                default:
                    break;
            }
            break;
        ///////////////// MouseWheelScrolled /////////////////
        case sf::Event::MouseWheelScrolled:
            std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
            std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
            std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
            if (event.mouseWheelScroll.delta > 0){
                std::cout << "Zoom in" << std::endl;
                viewport_actions_.SetFlag((EFlagValue)ViewportActions::ZOOM_IN_ONCE);
                    output = ViewportInputs::HANDLED;
            } else {
                std::cout << "Zoom out" << std::endl;
                viewport_actions_.SetFlag((EFlagValue)ViewportActions::ZOOM_OUT_ONCE);
                    output = ViewportInputs::HANDLED;
            }
            break;
        ///////////////// MouseClicked /////////////////
            break;
        ///////////////// Default /////////////////
        default:
            break;
    } //////////////// END event.type switch /////////////////

    return output;
}
///////////////////////////////// End GUI /////////////////////////////////

MapInputs Map::HandleInput(sf::Event& event){
    MapInputs output = MapInputs::NONE;

    switch (event.type){
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::Num1:{
                    std::cout << "Build workspace" << std::endl;
                    MakeBuilding(BuildingTypes::WORKSPACE, mouse_x_, mouse_y_);
                    break;}
                case sf::Keyboard::Scan::Num2:{
                    std::cout << "Build farm" << std::endl;
                    // create a farm
                    MakeBuilding(BuildingTypes::FARM, mouse_x_, mouse_y_);
                    break;}
                case sf::Keyboard::Scan::Num0:{
                    std::cout << "Make worker" << std::endl;
                    // create a worker
                    MakeWorker(mouse_x_, mouse_y_);
                    break;}
            }

        case sf::Event::MouseButtonReleased:
            std::cout << "mouse button: " << event.mouseButton.button << std::endl;
            std::cout << "mouse x: " << event.mouseButton.x << std::endl;
            std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            if (event.mouseButton.button == sf::Mouse::Left){
                std::cout << "Left mouse button released" << std::endl;
                // select a dynamic object
                SelectObject(mouse_x_, mouse_y_);
            }
            if (event.mouseButton.button == sf::Mouse::Right){
                std::cout << "Right mouse button released" << std::endl;
                SetAttentionAndMove(mouse_x_, mouse_y_);
            }
    }

    // selected workers take next precedence
    for (auto& dynamic_object_ptr : selected_dynamic_object_ptrs_){
        if (dynamic_object_ptr->dynamic_object_type_ == DynamicObjectTypes::WORKER){
        }
    }
    return output;
}
WorkerInputs Worker::HandleInput(sf::Event& event){
    WorkerInputs output = WorkerInputs::NONE;

    switch (event.type){
        case sf::Event::KeyReleased:
            switch (event.key.scancode){
                case sf::Keyboard::Scan::G:
                    std::cout << "Toggle gathering" << std::endl;
                    SetState(WorkerStates::GATHERIDLE);
                    break;
                case sf::Keyboard::Scan::C:
                    std::cout << "Toggle constructing" << std::endl;
                    SetState(WorkerStates::CONSTRUCTINGIDLE);
                    break;
                case sf::Keyboard::Scan::R:
                    std::cout << "Toggle crafting" << std::endl;
                    SetState(WorkerStates::CRAFTINGIDLE);
                    break;
                case sf::Keyboard::Scan::T:
                    std::cout << "Transfer Inventory" << std::endl;
                    TransferInventory();
                    break;
                case sf::Keyboard::Scan::E:
                    SetState(WorkerStates::EXECUTINGTASK);
                    break;
            }
    }

    return output;
}

HUD::HUD(){
}
// void HUD::Draw(sf::RenderWindow& window){
//     // draw everything else
//     inherited::Draw(window);

//     // draw the task manager on top (if visible)
//     task_manager_panel_ptr_->draw(window, sf::Transform::Identity);
// }
