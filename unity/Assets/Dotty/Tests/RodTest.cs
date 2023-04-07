using System.Collections;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using System.Collections.Generic;
using UnityEngine;

public class RodTest : MonoBehaviour
{
    public Dotty.World world; 

    public GameObject spherePrefab; 
    Transform[] sphereTransforms;

    int attractorId; 

    // Start is called before the first frame update
    void Start()
    {
        CreateString(); 
    }

    void CreateString(){
        world.SetGravity(9.81f); 
        world.SetGlobalDamping(0.01f); 
        world.SetSubsteps(4); 

        float height = 10;
        int H = 30; 
        sphereTransforms = new Transform[H]; 
        for(int j = 0; j < H; j++){
                
            Vector3 initialPosition = new Vector3(5, (float)(j) *  (height / H), 0);
            world.AddParticle(initialPosition, new Vector3(0, 0, 0), 1);

            GameObject go = Instantiate(spherePrefab, initialPosition, Quaternion.identity, transform ) as GameObject;
            sphereTransforms[j] = go.transform;   
            
            if(j < H-1){
                world.AddRod(j, j+1,   (height / H),   1f);
            }
            else{
                world.AddAnchorRod(j, initialPosition, 0, 1);
            }
        
        }
        attractorId = world.AddAttractor(new Vector3(0, 0, 0), 0, 0.1f, 100000, Dotty.Falloff.InvDist2);
        world.SetHasCollisionFloor(false); 
        world.SetCollisionFloor(0, 0, 0);
    }

    // Update is called once per frame
    void Update()
    {
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
                attractorOn = true; 
                hitPoint.z = -1; 
                world.SetAttractorPosition(attractorId, hitPoint); 
                world.SetAttractorStrength(attractorId, 100.0f);
            }
        }
        if(!attractorOn){
            world.SetAttractorStrength(attractorId, 0.0f);
        }
    }
}
