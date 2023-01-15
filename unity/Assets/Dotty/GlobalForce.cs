using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Dotty {
    public class GlobalForce : MonoBehaviour
    {
        private int internalId; 
        private bool started; 
        unsafe private GlobalForceNtv* ptr; 
        
        public World world; 

        public Vector3 direction;
        public float strength; 
        public Vector3 boundSize; 
        public BoundShapeType boundShape;
        public float boundThickness; 
        public Falloff boundFalloff;

        void AddInternal() {
            Vector3 position = transform.position; 
            internalId = world.AddGlobalForce(position, direction, strength, boundSize, boundShape, boundThickness, boundFalloff); 
            unsafe {
                ptr = world.GetGlobalForcePtr(internalId); 
            }
        }
        
        void RemoveInternal() {
            world.DestroyGlobalForce(internalId); 
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
    }
}