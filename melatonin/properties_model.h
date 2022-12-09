#pragma once
#include "helpers.h"
#include "model.h"

namespace melatonin
{
    class PropertiesModel : public juce::Component, public juce::ComponentListener
    {
    public:
        PropertiesModel(ComponentModel& _model): model(_model)
        {
            reset();

            //panel.setLookAndFeel(&propsLookAndFeel);
            addChildComponent(panel);
        }

        void paint (juce::Graphics& g) override
        {
            //just super simple separator line
            if(displayedComponent && panel.isVisible())
            {
                g.setColour (color::greyLineColor);
                g.drawHorizontalLine (0, padding, getRight() - padding);
            }
        }

        void resized() override
        {
            panel.setBounds(propsRectangle());
        }

        void displayComponent (Component* componentToDisplay)
        {
            if (displayedComponent)
            {
                displayedComponent->removeComponentListener (this);
            }

            displayedComponent = componentToDisplay;
            displayedComponent->addComponentListener (this);

            updateProperties();
        }

        // A selected component has been dragged or resized and this is our callback
        void componentMovedOrResized (Component& /*component*/, bool wasMoved, bool wasResized) override
        {
            if (wasResized || wasMoved)
            {
                updateProperties();
            }
        }

        void reset()
        {
            panel.clear();
            panel.setVisible(false);

            resized();
        }

    private:
        ComponentModel& model;

        juce::ToggleButton toggleButton;

        Component::SafePointer<Component> displayedComponent;

        juce::PropertyPanel panel{"Properties"};

        int padding = 30;
        int paddingToParent = 4;

        juce::Rectangle<int> parentComponentRectangle()
        {
            return getLocalBounds().reduced (padding, 0);
        }

        juce::Rectangle<int> propsRectangle()
        {
            return parentComponentRectangle().reduced(0, paddingToParent).withTrimmedTop (5);
        }

        void updateProperties()
        {
            panel.setVisible(true);
            panel.clear();
            auto props = createTextEditors();
            for(auto* p: props){
                p->setLookAndFeel(&getLookAndFeel());
            }
            panel.addProperties(props, 5);

            resized();
        }

        juce::Array<juce::PropertyComponent*> createTextEditors()
        {
            auto opaque = new juce::BooleanPropertyComponent (juce::Value(model.opaqueValue), "Opaque:", "");
            opaque->setEnabled(false);

            auto cachedImage = new juce::BooleanPropertyComponent (juce::Value(model.hasCachedImageValue), "CachedToImage:", "");
            cachedImage->setEnabled(false);

            auto focused = new juce::BooleanPropertyComponent (juce::Value(model.focused), "Focused:", "");
            focused->setEnabled(false);

            auto accessibilityHandled = new juce::BooleanPropertyComponent (juce::Value(model.accessibilityHandled), "Accessibility:", "");
            accessibilityHandled->setEnabled(false);

            return {
                new juce::TextPropertyComponent (juce::Value (model.lookAndFeel), "LookAndFeel:", 200, false, false),
                new juce::TextPropertyComponent (model.xValue, "X:", 5, false),
                new juce::TextPropertyComponent (model.yValue, "Y:", 5, false),
                new juce::TextPropertyComponent (model.widthValue, "Width:", 5, false),
                new juce::TextPropertyComponent (model.heightValue, "Height:", 5, false),
                new juce::TextPropertyComponent (juce::Value(model.fontValue), "Font:", 5, false, false),
                new juce::TextPropertyComponent (juce::Value(model.alphaValue), "Alpha:", 5, false, false),
                accessibilityHandled,
                focused,
                opaque,
                cachedImage
            };
        }
    };
}
