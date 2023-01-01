using System.Collections;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using System.Collections.Generic;
using UnityEngine;

public class ClothTest : MonoBehaviour
{
    Dotty.World world; 

    public GameObject spherePrefab; 
    Transform[] sphereTransforms;

    int attractorId; 

    // Start is called before the first frame update
    void Start()
    {
        world = new Dotty.World(); 
        CreateCloth(); 
    }

    void CreateCloth(){
        world.SetGravity(9.81f); 
        world.SetGlobalDamping(1.0f); 

        float width = 20; 
        float height = 10;
        int W = 80; 
        int H = 40; 
        sphereTransforms = new Transform[H*W]; 
        for(int j = 0; j < H; j++){
            for(int i = 0; i < W; i++){
                
                Vector3 initialPosition = new Vector3((float)(i) * (width / W), (float)(j) *  (height / H), 0);
                world.AddParticle(initialPosition, new Vector3(0, 0, 0), 10);

                GameObject go = Instantiate(spherePrefab, initialPosition, Quaternion.identity, transform ) as GameObject;
                sphereTransforms[W * (j) + (i)] = go.transform;   
                
                if(j < H-1){
                    world.AddRod(W * (j) + (i),   W * (j+1) + (i),   (width / W), 1);
                    if(i < W-1 ){
                        world.AddRod(W * (j) + (i),   W * (j) + (i+1),    (height / H), 1); 
                    }
                }
                else{
                    world.AddAnchorRod(W * (j) + (i), initialPosition, 0, 1);
                }
            }
        }
        attractorId = world.AddAttractor(new Vector3(0, 0, 0), 0, 0.1f, 100000, Dotty.Falloff.Squared);
        world.SetHasCollisionFloor(true); 
        world.SetCollisionFloor(0, 0, 0);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        
        world.Update();

        Vector3 newPosition = new Vector3(0, 0, 0);
        
        unsafe {
            Dotty.Particle* ptr = world.GetParticlesPtr(); 
            int bound = world.GetParticlesPoolBound(); 
            for(int i = 0; i < bound; i++){
                Dotty.Particle p = *ptr; 
                newPosition.x = p.position.x;
                newPosition.y = p.position.y;
                newPosition.z = p.position.z;
                sphereTransforms[i].localPosition = newPosition; 
                ptr++; 
            }
        }

        bool attractorOn = false; 
        if (Input.GetMouseButton(0)){
            var plane = new Plane(new Vector3(0, 0, 1), 0); 
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            float enter = 0.0f;
            if (plane.Raycast(ray, out enter)){
                Vector3 hitPoint = ray.GetPoint(enter);
                Debug.Log("Pressed primary button. " + hitPoint.x + " " + hitPoint.y);
                attractorOn = true; 
                hitPoint.z = -1; 
                world.SetAttractorPosition(attractorId, hitPoint); 
                world.SetAttractorStrength(attractorId, 1.0f);
            }
        }
        if(!attractorOn){
            world.SetAttractorStrength(attractorId, 0.0f);
        }
    }
}
