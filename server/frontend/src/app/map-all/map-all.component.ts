import { Component, OnInit } from '@angular/core';

import * as olsource from 'ol/source';
import Map from 'ol/Map';
import TileLayer from 'ol/layer/Tile';
import View from 'ol/View';
import { fromLonLat } from 'ol/proj';
import Feature from 'ol/Feature';
import { Point } from 'ol/geom';
import { Vector } from 'ol/layer';

import { DeviceService } from '../service/device.service';
import { Device } from '../device/device.component';


@Component({
  selector: 'map-all',
  templateUrl: './map-all.component.html',
  styleUrls: ['./map-all.component.css']
})
export class MapAllComponent implements OnInit {

  constructor(private service: DeviceService) { }

  map!: Map;
  zoom: number = 7;
  latitude: number = 39.749926;
  longitude: number = -86.148711;

  ngOnInit(): void {
    let position = fromLonLat([this.longitude, this.latitude]);
    this.map = new Map({
      target: 'all_device_map',
      layers: [
        new TileLayer({source: new olsource.OSM()})
      ],
      view: new View({
        center: position,
        zoom: this.zoom
      })
    });

    this.service.allDeviceInfo()
      .subscribe(Response => {
        let markers = new Array;
        Response.forEach((device: Device) => {
          if (device.latitude != null && device.longitude != null){
            let device_location = fromLonLat([device.longitude, device.latitude]);
            markers.push(new Feature({
              geometry: new Point(device_location)
            }))  
          }
        })
        let layer = new Vector({
          source: new olsource.Vector({features: markers})  
        })
        this.map.addLayer(layer);
      });

  }

}
