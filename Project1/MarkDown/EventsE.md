
---
# Events

### **To test a key is pressed**

` Event::keypressed cout<<"Key is presses";`


---
---
# Matching type

---
`event.type==Event::` `Type of Evenet`
--- 

**Type of event**
- `Closed` - *closing window*

- `KeyPressed` - *Key pressed*

- `KeyReleased` - *Key Released*

- `MouseButtonPressed` - *Mouse button pressed*

- `MouseWheelMoved` - *Mouse wheel moved*

---
`event.key.code` == `sf::Keyboard::A`
---

A can be any keyboard button 

---
`event.key.code` == `sf::Mouse::Left`
---


Left can be Right,Middle 

---

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A) std::cout << "A is pressed" << std::endl;
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::A) std::cout << "A is released"std::endl;
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Middle) std::cout << "middle mouse button pressed" << std::endl;
                if (event.mouseButton.button == sf::Mouse::Right) std::cout << "right mouse button pressed" << std::endl;
                if (event.mouseButton.button == sf::Mouse::Left) std::cout << "left mouse button pressed" << std::endl;
            }

             if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Middle) std::cout << "middle mouse button released" << std::endl;

                if (event.mouseButton.button == sf::Mouse::Right) std::cout << "right mouse button released" << std::endl;

                if (event.mouseButton.button == sf::Mouse::Left) std::cout << "left mouse button released" << std::endl;
            } 

            if(sf::Event::MouseWheelMoved== event.type)
            {
                 std::cout<<event.mouseWheel.delta<<std::endl;
            } 

            if(sf::Event::MouseMoved==event.type)
            {
                std::cout<<event.mouseMove.x<<" "<<event.mouseMove.y;
            }