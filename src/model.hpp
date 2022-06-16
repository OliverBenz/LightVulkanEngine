#pragma once

#include "object.hpp"

#include <string>
#include <vector>

class Model : public Object {
public:
	Model(const std::string pathModel, const std::string pathTexture);

private:
	void loadModel();

private:
	std::string m_pathModel;
	std::string m_pathTexture;
};
