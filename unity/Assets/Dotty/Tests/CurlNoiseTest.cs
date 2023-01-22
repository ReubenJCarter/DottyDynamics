using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CurlNoiseTest : MonoBehaviour
{
    public Dotty.World world; 

    public GameObject spherePrefab; 
    Transform[] sphereTransforms;

    int attractorId; 

    // Start is called before the first frame update
    void Start()
    {
        world.SetGlobalDamping(1f); 
        world.SetGravity(0); 

        int count = 10000; 
        sphereTransforms = new Transform[count]; 
        for(int j = 0; j < count; j++){
                
            Vector3 initialPosition = new Vector3(Random.value * 10, Random.value * 10, Random.value * 10);
            world.AddParticle(initialPosition, new Vector3(0, 0, 0), 1);

            GameObject go = Instantiate(spherePrefab, initialPosition, Quaternion.identity, transform ) as GameObject;
            sphereTransforms[j] = go.transform;   
        
        }
        //int curlNoiseId = world.AddNoiseField(Dotty.NoiseType.SimplexCurl, 50f, 10.0f, true); 
        //world.AddNoiseField(Dotty.World.NoiseType.Simplex, 0.1f, 40.0f, false); 
        //world.AddAttractor(new Vector3(5, 5, 5), 0.1f, 20, 100000, Dotty.World.Falloff.LinearWell); 

        attractorId = world.AddAttractor(new Vector3(0, 0, 0), 10, 1f, 100000, Dotty.Falloff.InvDist2);
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
                world.SetAttractorStrength(attractorId, 10.0f);
            }
        }
        if(!attractorOn){
            world.SetAttractorStrength(attractorId, 0.0f);
        }

    }
}
