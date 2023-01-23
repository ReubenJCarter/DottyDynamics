using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Dotty {
    public class StrangeAttractor : MonoBehaviour
    {
        private int internalId; 
        private bool started; 
        unsafe private StrangeAttractorNtv* ptr; 
        
        public World world; 

        public float scale = 1; 
        public StrangeAttractorType type = StrangeAttractorType.Aizawa; 
        public float strength = 10; 
        public float minDistance = 0.1f; 
        public float maxDistance = 1000; 
        public Falloff falloff = Falloff.InvDist2; 

        void AddInternal() {
            Vector3 position = transform.position; 
            internalId = world.AddStrangeAttractor(position, scale, type, strength, minDistance, maxDistance, falloff); 
            unsafe {
                ptr = world.GetStrangeAttractorPtr(internalId); 
            }
        }
        
        void RemoveInternal() {
            world.DestroyStrangeAttractor(internalId); 
        }

        void Start() {
            if(world == null){
                world = World.instance; 
            }
            
            started = true; 
            AddInternal(); 
        }

        void OnEnable() {
            if(started) {
                AddInternal(); 
            }
        }

        void OnDisable() {
            RemoveInternal(); 
        }

        // Update is called once per frame
        void Update() {
            Vector3 position = transform.position; 
            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z;
                (*ptr).scale = scale; 
                (*ptr).type = type; 
                (*ptr).strength = strength; 
                (*ptr).minDist = minDistance; 
                (*ptr).maxDist = maxDistance; 
                (*ptr).falloff = falloff; 
            } 
        }
    }
}
