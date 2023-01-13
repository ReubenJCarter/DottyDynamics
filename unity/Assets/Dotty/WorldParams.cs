using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Dotty{
    public class WorldParams : MonoBehaviour
    {
        private bool started;
        unsafe private WorldParamsNtv* ptr; 

        public World world; 

        [Header("Timing & Performance")]
        public float timestep; 
        public int substeps; 

        [Header("Global Environment")]
        [Range(0.0f, 1.0f)]
        public float globalDamping;
        public float gravity; 

        [Header("Floor Boundry")]
        public bool hasCollisionFloor; 
        public float collisionFloorHeight; 
        public float collisionFloorKineticFriction; 
        public float collisionFloorStaticFriction; 
        
        void UpdateInternal() {
            unsafe {
                ptr = world.GetWorldParamsPtr(); 
                (*ptr).timestep = timestep;
                (*ptr).substeps = substeps;
                (*ptr).globalDamping = globalDamping;
                (*ptr).gravity = gravity;
                (*ptr).hasCollisionFloor = hasCollisionFloor;
                (*ptr).collisionFloorHeight = collisionFloorHeight;
                (*ptr).collisionFloorKineticFriction = collisionFloorKineticFriction; 
                (*ptr).collisionFloorStaticFriction = collisionFloorStaticFriction; 
            }
        }

        // Start is called before the first frame update
        void Start()
        {
            UpdateInternal(); 
        }

        // Update is called once per frame
        void Update()
        {
            UpdateInternal(); 
        }
    }
}