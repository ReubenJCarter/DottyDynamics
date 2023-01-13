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
        public float timestep = 0.0166666f; 
        public int substeps = 1; 

        [Header("Global Environment")]
        [Range(0.0f, 1.0f)]
        public float globalDamping = 1;
        public float gravity = 9.81f ; 

        [Header("Floor Boundry")]
        public bool hasCollisionFloor = false; 
        public float collisionFloorHeight = 0; 
        public float collisionFloorKineticFriction = 0; 
        public float collisionFloorStaticFriction = 0; 
        
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
            if(world == null){
                world = World.instance; 
            }
            UpdateInternal(); 
        }

        // Update is called once per frame
        void Update()
        {
            UpdateInternal(); 
        }
    }
}