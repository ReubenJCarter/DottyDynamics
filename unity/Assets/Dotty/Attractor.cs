using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Dotty{
    public class Attractor : MonoBehaviour
    {
        private int internalId; 
        unsafe private AttractorNtv* ptr; 
        
        public World world; 

        public float strength = 10; 
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
            internalId = world.AddAttractor(position, strength, minDistance, maxDistance, falloff); 
            unsafe{
                ptr = world.GetAttractorPtr(internalId); 
            }
        }

        void OnDisable()
        {
            world.DestroyAttractor(internalId); 
        }

        // Update is called once per frame
        void Update()
        {
            Vector3 position = transform.position; 
            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z;
                (*ptr).strength = strength; 
                (*ptr).minDist = minDistance; 
                (*ptr).maxDist = maxDistance; 
                (*ptr).falloff = falloff; 
            } 
        }
    }
}