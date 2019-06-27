#pragma once
namespace LearnEngine {

class NonSkinModel;

class VertexShader;

std::unique_ptr<NonSkinModel> loadNonSkinModel(const char* filePath);

}

