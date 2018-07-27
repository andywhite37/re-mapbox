module Config = Mapbox_GL_Map_Config;

module FitOptions = {
  type t_js = {.
    "duration": Js.undefined(int) /* in ms */
  };

  [@bs.obj] external makeJs : (
    ~duration: int=?,
    unit
  ) => _ = ""
};

type eventData = Js.Dict.t(Js.Json.t);

type t = {.
  [@bs.meth] "setCenter": Mapbox_GL_LngLat.t => t,
  [@bs.meth] "setZoom": float => t,
  [@bs.meth] "fitBounds": Mapbox_GL_LngLatBounds.t => FitOptions.t_js => t,
  [@bs.meth] "fitBounds__eventData": Mapbox_GL_LngLatBounds.t => FitOptions.t_js => Js.Dict.t(Js.Json.t) => t,
  [@bs.meth] "on": (string, eventData => unit) => t,
  [@bs.meth] "remove": unit => unit,
  [@bs.meth] "resize": unit => unit,
  [@bs.meth] "getBounds": unit => Mapbox_GL_LngLatBounds.t,
  [@bs.meth] "addLayer": Mapbox_GL_Layer.t_js => t,
  [@bs.meth] "removeLayer": string => t, /* TODO: docs don't specify return */
  [@bs.meth] "addSource": string => Mapbox_GL_Source.t_js => t,
  [@bs.meth] "removeSource": string => t
};

[@bs.new][@bs.module "mapbox-gl/dist/mapbox-gl.js"]
external createExn: Config.jsObj => t = "Map";

let create = cfg =>
  try (Belt.Result.Ok(createExn(cfg))) {
  | _ => Belt.Result.Error("Failed to initialize MapboxGL")
  };