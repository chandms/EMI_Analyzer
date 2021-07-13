import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';

import * as olsource from 'ol/source';
import Map from 'ol/Map';
import TileLayer from 'ol/layer/Tile';
import View from 'ol/View';
import Feature from 'ol/Feature';
import { Point } from 'ol/geom';
import { fromLonLat } from 'ol/proj';
import { Vector } from 'ol/layer';
import { DeviceService } from '../service/device.service';


@Component({
  selector: 'location',
  templateUrl: './location.component.html',
  styleUrls: ['./location.component.css']
})
export class LocationComponent implements OnInit {

  constructor(private route: ActivatedRoute, private service: DeviceService) { }

  device_name: string = '';
  device_id: number = 0;
  latitude: number = 40.421488;
  longitude: number = -86.916631;
  mac_address: string = '';
  last_update: Date = new Date();
  
  marker = new Feature({
    geometry: new Point(fromLonLat([this.longitude, this.latitude]))
  });

  layer = new Vector({
    source: new olsource.Vector({features: [this.marker]})
  });

  map!: Map;
  zoom: number = 15;

  ngOnInit(): void {
    this.device_name = this.route.snapshot.params.device_name;
    this.service.getDeviceInfo(this.device_name)
      .subscribe(Response => {
        console.log(Response);
        this.device_id = Response.device_id
        this.last_update = Response.last_updated;
        this.longitude = Response.longitude;
        this.latitude = Response.latitude;
        if ((this.latitude == null) || (this.longitude == null)) {
         this.latitude = 40.421488;
         this.longitude = -86.916631;
        }
        let new_position = fromLonLat([this.longitude, this.latitude]);
        this.map.getView().setCenter(new_position);
        this.marker.setGeometry(new Point(new_position));
      })
    
      let position = fromLonLat([this.longitude, this.latitude]);
      this.map = new Map({
        target: 'device_map',
        layers: [
          new TileLayer({source: new olsource.OSM()})
        ],
        view: new View({
          center: position,
          zoom: this.zoom
        })
      });
      
      this.map.addLayer(this.layer);
      

  }

  changeLocation() {
    let new_position = fromLonLat([this.longitude, this.latitude]);
    this.map.getView().setCenter(new_position);
    this.marker.setGeometry(new Point(new_position));
    this.service.updateLocation(this.device_id, this.latitude, this.longitude)
      .subscribe(Response => {
        console.log(Response);
      });
  }
}
