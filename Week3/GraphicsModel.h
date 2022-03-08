#pragma once

class GraphicsModel {
public:
    virtual ~GraphicsModel();

    virtual void draw() = 0;
};

class NullGraphicsModel final : public GraphicsModel {
public:
    virtual void draw() override;
};
