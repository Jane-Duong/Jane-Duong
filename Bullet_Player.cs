using UnityEngine;

public class Bullet_Player : MonoBehaviour
{
    public float lifeTime = 2.5f;
    private void OnEnable()
    {
        Invoke(nameof(Kill), lifeTime);
    }
    private void OnDisable()
    {
        CancelInvoke(); 
    }
    void Kill()
    {
        Destroy(gameObject);
    }
    private void OnTriggerEnter2D(Collider2D other)
    {
        if(other.CompareTag("Enemy"))
        {
            EnemyShip enemy = other.GetComponent<EnemyShip>(); 
            if (enemy != null)
            {
                enemy.OnHit(); 
            }
            Destroy(gameObject);
        }
        if(other.gameObject.tag == "Boundary")
        {
            Destroy(gameObject); 
        }
    }
}
