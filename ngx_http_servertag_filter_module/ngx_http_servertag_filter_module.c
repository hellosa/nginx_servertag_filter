
/*
 * Copyright (C) Hello Chan
 * Date: 2010-08 
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>



static ngx_int_t ngx_http_servertag_filter_init(ngx_conf_t *cf);

static void* ngx_http_servertag_filter_create_srv_conf(ngx_conf_t *cf);

static char* ngx_http_servertag_filter_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);


typedef struct {
    ngx_str_t   tag;
} ngx_http_servertag_filter_srv_conf_t;


static ngx_command_t ngx_http_servertag_filter_commands[] = {
    { ngx_string("server_tag"),
      NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_servertag_filter_srv_conf_t, tag),
      NULL },

    ngx_null_command
};

static ngx_http_module_t  ngx_http_servertag_filter_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_servertag_filter_init,     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_servertag_filter_create_srv_conf,                                  /* create server configuration */
    ngx_http_servertag_filter_merge_srv_conf,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_servertag_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_servertag_filter_module_ctx, /* module context */
    ngx_http_servertag_filter_commands,                                  /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;


static ngx_int_t
ngx_http_servertag_header_filter(ngx_http_request_t *r)
{
    ngx_http_servertag_filter_srv_conf_t *tfcf ;
    tfcf = ngx_http_get_module_srv_conf(r, ngx_http_servertag_filter_module);

    if (ngx_strcmp(tfcf->tag.data , "") == 0 ){
        return ngx_http_next_header_filter(r);
    }


    r->headers_out.server = ngx_list_push(&r->headers_out.headers) ;
    r->headers_out.server->hash = 1;
    r->headers_out.server->key.len = sizeof("Server") - 1;
    r->headers_out.server->key.data = (u_char *) "Server";
    r->headers_out.server->value.len = tfcf->tag.len ;
    r->headers_out.server->value.data = tfcf->tag.data ;

    return ngx_http_next_header_filter(r);
}


static ngx_int_t
ngx_http_servertag_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_servertag_header_filter;

    return NGX_OK;
}



static void *
ngx_http_servertag_filter_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_servertag_filter_srv_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_servertag_filter_srv_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }
    return conf;
}


static char *
ngx_http_servertag_filter_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_servertag_filter_srv_conf_t *prev = parent;
    ngx_http_servertag_filter_srv_conf_t *conf = child;

    ngx_conf_merge_str_value(conf->tag, prev->tag, "");

    return NGX_CONF_OK;
}

