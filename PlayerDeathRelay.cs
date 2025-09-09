using System.Collections.Specialized;
using UnityEngine;

public class PlayerDeathRelay : MonoBehaviour
{
    public System.Action onDied;
    bool notified = false;
    private void OnDisable()
    {
        Notify(); 
    }
    private void OnDestroy()
    {
        Notify();
    }
    void Notify()
    {
        if (notified)
            return; 
        notified = true;
        onDied?.Invoke(); 
    }
}
