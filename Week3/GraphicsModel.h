#pragma once

#include <memory>

class GraphicsModel {
public:
	using Ptr = std::shared_ptr<GraphicsModel>;
	using UPtr = std::unique_ptr<GraphicsModel>;
	using WPtr = std::weak_ptr<GraphicsModel>;

    virtual ~GraphicsModel();

    virtual void draw() = 0;
};

class NullGraphicsModel final : public GraphicsModel {
public:
	using Ptr = std::shared_ptr<NullGraphicsModel>;
	using UPtr = std::unique_ptr<NullGraphicsModel>;
	using WPtr = std::weak_ptr<NullGraphicsModel>;

    virtual void draw() override;
};
