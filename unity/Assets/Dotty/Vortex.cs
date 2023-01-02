using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Dotty{
    public class Vortex : MonoBehaviour
    {
        private int internalId; 
        unsafe private VortexNtv* ptr; 
        
        public World world; 

        public float strength = 1; 
        public float minDistance = 0.1f; 
        public float maxDistance = 1000; 
        public Falloff falloff = Falloff.Squared; 


        // Start is called before the first frame update
        void Start()
        {
        }

        void OnEnable()
        {
            Vector3 position = transform.position; 
            Vector3 normal = transform.rotation * Vector3.up;
            internalId = world.AddVortex(position, normal, strength, minDistance, maxDistance, falloff); 
            unsafe{
                ptr = world.GetVortexPtr(internalId); 
            }
        }

        void OnDisable()
        {
            world.DestroyVortex(internalId); 
        }

        // Update is called once per frame
        void Update()
        {
            Vector3 position = transform.position; 
            Vector3 normal = transform.rotation * Vector3.up;
            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z;
                (*ptr).normal.x = normal.x;
                (*ptr).normal.y = normal.y;
                (*ptr).normal.z = normal.z;
                (*ptr).strength = strength; 
                (*ptr).minDist = minDistance; 
                (*ptr).maxDist = maxDistance; 
                (*ptr).falloff = falloff; 
            } 
        }
    }
}