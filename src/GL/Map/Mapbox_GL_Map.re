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

type callback('a) = (Js.null(string), 'a) => unit;

/* This thing is confusing. It's probably the original object you provided when
   creating the source, but with extra methods added to it, depending on what
   "type" of source it is. For now, We just expose all methods to you.
*/
type source = {.
  [@bs.meth] "setData": Js.Json.t => source,
  [@bs.meth] "getClusterExpansionZoom": (float, callback(float)) => source,
  [@bs.meth] "getClusterChildren": (float, callback(Js.Json.t)) => source,
  [@bs.meth] "getClusterLeaves": (float, float, float, callback(Js.Json.t)) => source
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
  [@bs.meth] "getLayer": string => Js.undefined(Mapbox_GL_Layer.t_js),
  [@bs.meth] "removeLayer": string => t, /* TODO: docs don't specify return */
  [@bs.meth] "addSource": string => Mapbox_GL_Source.t_js => t,
  [@bs.meth] "getSource": string => Js.undefined(source),
  [@bs.meth] "removeSource": string => t
};

[@bs.new][@bs.module "mapbox-gl/dist/mapbox-gl.js"]
external createExn: Config.jsObj => t = "Map";

let create = cfg =>
  try (Belt.Result.Ok(createExn(cfg))) {
  | _ => Belt.Result.Error("Failed to initialize MapboxGL")
  };
