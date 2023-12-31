#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(glm::vec3 gravity, int iterations) {
	this->gravity = gravity;
	this->iterations = iterations;
}
void PhysicsWorld::Step(float deltaTime)
{
	for (int iter = 0; iter < iterations; iter++)
	{
		for (int i = 0; i < bodies.size(); i++) {
			if (!bodies[i]->isStatic)
				bodies[i]->Step(deltaTime/iterations);
		}
	}

	for (int iter = 0; iter < iterations; iter++)
	{

		for (int i = 0; i < bodies.size(); i++) {
			if (bodies[i]->isTrigger) {
				continue;
			}

			for (int l = i + 1; l < bodies.size(); l++) {
				if (bodies[l]->isTrigger) {
					continue;
				}

				vec2 mtv;
				if (CheckCollision(*bodies[i]->GetCollider(), *bodies[l]->GetCollider(), mtv)) {
					if (mtv == vec2(0.0f)) {
						continue;
					}

					vec2 normal = normalize(mtv);
					float depth = length(mtv);


					PhysicsBody* bodyA = bodies[i];
					PhysicsBody* bodyB = bodies[l];

					// Get what bodies are static
					bool isAStatic = bodyA->isStatic;
					bool isBStatic = bodyB->isStatic;

					// Change their position based on MTV
					if (!isAStatic && !isBStatic) {
						*bodyA->position += mtv / 2.0f;
						*bodyB->position -= mtv / 2.0f;
					}
					if (!isAStatic && isBStatic) {
						*bodyA->position += mtv;
					}
					if (isAStatic && !isBStatic) {
						*bodyB->position -=  mtv;
					}

					

					//vec2 point1 = bodyA->GetCollider().GetSupportPoint(-normal);
					//vec2 point2 = bodyB->GetCollider()-GetSupportPoint(normal);
					int sizeContactPoints = 2;
					
					// calculate relative velocity
					vec2 relVel = bodyB->velocity - bodyA->velocity;
					
					// calculate impulse magnitude
					float e = std::min(bodyA->restitution, bodyB->restitution); // coefficient of restitution
					float j = -(1 + e) * dot(relVel, normal) / ((bodyA->isStatic ? 0 : 1 / bodyA->mass) + (bodyB->isStatic ? 0 : 1 / bodyB->mass) + 0.000000000001f);
					
					// friction stuff
					vec2 tangent = vec2(-normal.y, normal.x);
					vec2 relVelTangent = relVel - dot(relVel, normal) * normal;
					
					float friction = std::min(bodyA->friction, bodyB->friction);
					float jt = -dot(relVelTangent, tangent) / ((bodyA->isStatic ? 0 : 1 / bodyA->mass) + (bodyB->isStatic ? 0 : 1 / bodyB->mass) + 0.000000000001f);
					jt /= sizeContactPoints; // average for multiple contact points
					
					if (std::abs(jt) > std::abs(j) * friction) {
						jt = std::copysign(std::abs(j) * friction, jt);
					}
					
					// apply impulse to the bodies
					if (!isAStatic && !isBStatic) {
						bodyA->velocity -= (j * normal + jt * tangent) / bodyA->mass;
						bodyB->velocity += (j * normal + jt * tangent) / bodyB->mass;
					}								   
					if (!isAStatic && isBStatic) {	  
						bodyA->velocity -= (j * normal + jt * tangent) / bodyA->mass;
					}								   
					if (isAStatic && !isBStatic) {	   
						bodyB->velocity += (j * normal + jt * tangent) / bodyB->mass;
					}
				}
			}	
		}
		
	}
	
}
bool PhysicsWorld::TouchingLayer(PhysicsBody* body, int layer) {

	UpdateLayerBodies();

	// There is no such layer or more accuraetly no bodies with this layer
	if (layeredBodies.find(layer) == layeredBodies.end()) {
		return false;
	}
	vec2 mtv;
	for (size_t l = 0; l < layeredBodies[layer].size(); l++) {

		if (CheckCollision(*layeredBodies[layer][l]->GetCollider(), *body->GetCollider(),mtv)) {
			return true;
		}
	}
	return false;
}
void PhysicsWorld::UpdateLayerBodies() {
	layeredBodies.clear();
	layers.clear();
	for (size_t i = 0; i < bodies.size(); i++) {
		int l = bodies[i]->layer;
		layeredBodies[l].push_back(bodies[i]);
		layers.push_back(l);
	}
}
void PhysicsWorld::SetGravity(vec3 gravity)
{
	this->gravity = gravity;
	for (PhysicsBody* body : bodies) {
		body->SetGravity(gravity);
	}
}

vec3 PhysicsWorld::GetGravity()
{
	return gravity;
}

void PhysicsWorld::AddBody(PhysicsBody* body) {
	for (PhysicsBody* existingBody : bodies) {
		if (existingBody == body) {
			// Body already exists, so return without adding it again
			return;
		}
	}

	body->SetGravity(gravity);
	bodies.push_back(body);
}

void PhysicsWorld::RemoveBody(PhysicsBody* body)
{
	for (auto it = bodies.begin(); it != bodies.end(); ++it) {
		if (*it == body) {
			bodies.erase(it);
			return;
		}
	}
}

vector<PhysicsBody*> PhysicsWorld::GetBodies() {
	return bodies;
}

