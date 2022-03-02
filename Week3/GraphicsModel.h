#pragma once

class GraphicsModel {
public:
    virtual ~GraphicsModel();

    virtual void draw() = 0;
};

class NullGraphicsModel : public GraphicsModel {
public:
    virtual void draw() override;
};
