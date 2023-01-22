using System.Collections;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using System.Collections.Generic;
using UnityEngine;

public class SceneTest : MonoBehaviour
{
    public Dotty.World world; 

    public GameObject spherePrefab; 
    Transform[] sphereTransforms;

    // Start is called before the first frame update
    void Start()
    {
        int count = 10000; 
        sphereTransforms = new Transform[count]; 
        for(int j = 0; j < count; j++){
                
            Vector3 initialPosition = new Vector3(Random.value * 10, Random.value * 10, Random.value * 10);
            world.AddParticle(initialPosition, new Vector3(0, 0, 0), 1);

            GameObject go = Instantiate(spherePrefab, initialPosition, Quaternion.identity, transform ) as GameObject;
            sphereTransforms[j] = go.transform;   
        
        }
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
    }
}
