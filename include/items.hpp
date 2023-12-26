// include guard
#ifndef ITEMS_HPP
#define ITEMS_HPP

#include "globals.hpp"
#include "BitWise.hpp"
#include "resources.hpp"
#include <map>

// chain ResourceTypes and ItemTypes using this method: https://stackoverflow.com/a/18344942
enum class ItemCategories {
    NONE,
    RESOURCE,
    FOOD,  
};
enum class ItemTypes {
    NONE,
    CORNSEED,

    // copy paste from ResourceTypes
    LUMBER = 128,
    STONE,
    WATER,
    GRASS,
    CORN,
};

// define mapping from ResourceTypes to ItemTypes
extern std::map<ResourceTypes, ItemTypes> RESOURCE_TO_ITEM_MAP;

class Item
{
    public:
        ItemTypes item_type_;
        ItemCategories item_category_{ItemCategories::NONE};
        double amount_{0.0};

        Item(){ItemTypes::NONE;}
        Item(ItemTypes item_type, double amount){item_type_ = item_type; amount_ = amount;}
        void SetAmount(double amount){
            double new_amount = std::max(0.0, amount);
            amount_ = new_amount;
        }
        double GetAmount(){return amount_;}
        void AddAmount(double amount){amount_ += amount;}
        void RemoveAmount(double amount){
            SetAmount(amount_ - amount);
            }
};

class ResourceItem: public Item
{
    public:
        ResourceTypes resource_type_;

        ResourceItem(){ResourceTypes::NONE;}
        ResourceItem(ResourceTypes resource_type, double amount){resource_type_ = resource_type; amount_ = amount;}
};

class FoodItem: public Item
{
    public:
        double nutrient_amount_{0.0}; // per unit of food
        FoodItem(){};
};

class CornItem: public FoodItem
{
    public:
        CornItem(double amount){item_type_ = ItemTypes::CORN; item_category_ = ItemCategories::FOOD, amount_ = amount; nutrient_amount_ = 10.0;}
};

// TODO: make item factory
class ItemFactory
{
    public:
        static std::shared_ptr<Item> MakeItem(ItemTypes item_type, double amount){
            switch (item_type)
            {
                case ItemTypes::CORN:
                    return std::make_shared<CornItem>(amount);
                    break;
                default:
                    return std::make_shared<Item>(item_type, amount);
                    break;
            }
        }
};

#endif // ITEMS_HPP