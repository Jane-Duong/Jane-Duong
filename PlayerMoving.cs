using UnityEngine;
using UnityEngine.PlayerLoop;

public class PlayerMoving : MonoBehaviour
{
    [Header("Move")]
    public float moveSpeed = 7f;
    public float hInput;

    [Header("Entrance")]
    public float entranceStartY = -6.5f;
    public float entranceTargetY = -2.5f;
    public float entranceTime = 1.0f;
    public bool invincibleDuringEntrance = true;

    [Header("Refs")]
    public Animator anim;
    public Collider2D hitbox;



    void Start()
    {

    }
    private void Awake()
    {
        if (!hitbox) hitbox = GetComponent<Collider2D>();
        if (!anim) anim = GetComponent<Animator>();
    }
    private void OnEnable()
    {
        // Going from the bottom
        Vector3 p = transform.position;
        p.y = entranceStartY;
        transform.position = p;
        // Disable hitbox if invincible
        if (invincibleDuringEntrance && hitbox)
            hitbox.enabled = false;
        StartCoroutine(Entrance());
    }
    System.Collections.IEnumerator Entrance()
    {
        float t = 0f;
        Vector3 start = transform.position;
        Vector3 end = new Vector3(start.x, entranceTargetY, start.z);
        while (t < 1)
        {
            t += Time.deltaTime / entranceTime;
            transform.position = Vector3.Lerp(start, end, Mathf.SmoothStep(0, 1, t));
            yield return null;
        }
        if (invincibleDuringEntrance && hitbox)
            hitbox.enabled = true;
    }
    // Update is called once per frame
    void Update()
    {
        // moving right/left
        hInput = Input.GetAxisRaw("Horizontal");
        transform.Translate(Vector2.right * hInput * moveSpeed * Time.deltaTime, Space.World);
        // Animation: Idle <-> move 
        if (anim)
        {
            anim.SetBool("isMoving", hInput != 0);
        }
        // Attack with space 
        if (Input.GetButtonDown("Fire1"))
        {
            anim.SetTrigger("isAttack"); 
        }

    }
    
    public void OnDestroyed()
    {
        if (anim)
        {
            anim.SetTrigger("isDead"); 
        }
        if (hitbox) hitbox.enabled = false;  // turn-off hitbox
    }
    public void OnExplosionEnd()
    {
        Destroy(gameObject); 
    }
    
    public void OnHit()
    {
        Debug.Log("Player hit");
        if (anim != null)
        {
            anim.SetTrigger("isDead"); 
        }
        if (hitbox != null)
        {
            hitbox.enabled |= false;
        }
        // game-over 
        Destroy(gameObject, 1.5f); 
    }
    
}
