ps.1.1

;This is the light texture
tex t0

;This is the same texture, but this shader only uses the
;alpha channel
tex t1

mul r0, t0, t1.a
mul r0, r0, v0