using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Dotty {
    abstract public class BaseComponent : MonoBehaviour{
        protected int internalId; 
        protected bool started; 
        protected bool created; 
        
        
        public World world; 

        protected abstract void AddInternal(); 
        protected abstract void RemoveInternal(); 

        void Start() {
            if(world == null){
                world = World.instance; 
            }

            if(world == null){
                return;
            }
            
            started = true; 

            world.worldCreated.AddListener(OnWorldCreated); 
            world.worldDestroyed.AddListener(OnWorldDestroyed); 

            AddInternal(); 
            created = true; 
        }

        void OnWorldCreated(){

        }

        void OnWorldDestroyed(){
            created = false;
            started = false; 
        }

        void OnEnable() {
            if(started && !created) {
                AddInternal(); 
                created = true; 
            }
        }

        void OnDisable() {
            if(started && created){
                RemoveInternal(); 
                created = false;
            } 
        }

        void OnDestroy(){
            if(started){
                world.worldDestroyed.RemoveListener(OnWorldDestroyed);
                world.worldDestroyed.RemoveListener(OnWorldDestroyed);
            }
        }
    }
}
