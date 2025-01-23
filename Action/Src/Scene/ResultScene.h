#ifndef RESULT_SCENE_H_
#define RESULT_SCENE_H_

#include "Common/GameData.h"
#include "World/World.h"
#include "Actor/Actor.h"
#include <vector>;
#include <string>



class ResultScene : public Actor {

public:

	ResultScene(World* world);

	void update(float delta_time);

	void draw() const;


};

#endif // !RESULT_SCENE_H_
