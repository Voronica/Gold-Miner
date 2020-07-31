//
//  Level1.hpp
//  GoldMiner
//
//  Created by Shucong Zhou on 2020/7/31.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Scene.h"

class Level1 : public Scene {
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
